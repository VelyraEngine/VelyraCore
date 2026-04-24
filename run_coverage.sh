#!/bin/bash
# Code Coverage Helper Script for VelyraCore
# This script simplifies running code coverage analysis

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR="cmake-build-coverage"
FULL_BUILD_DIR="$SCRIPT_DIR/$BUILD_DIR"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_usage() {
    echo "Usage: $0 [COMMAND] [OPTIONS]"
    echo ""
    echo "Commands:"
    echo "  setup              Configure and build with coverage enabled"
    echo "  html               Generate HTML coverage report (default)"
    echo "  summary            Display coverage summary in terminal"
    echo "  detailed           Display detailed coverage report in terminal"
    echo "  xml                Generate XML coverage report (for CI/CD)"
    echo "  json               Generate JSON coverage report"
    echo "  clean              Clean coverage data"
    echo "  view               Generate HTML report and open in browser"
    echo "  all                Generate all report formats"
    echo "  help               Show this help message"
    echo ""
    echo "Options:"
    echo "  --build-dir DIR    Use custom build directory (default: cmake-build-coverage)"
    echo ""
    echo "Examples:"
    echo "  $0 setup           # First time setup"
    echo "  $0 html            # Generate HTML report"
    echo "  $0 view            # Generate and open HTML report in browser"
    echo "  $0 summary         # Quick coverage summary"
}

check_tools() {
    local missing_tools=()
    
    if ! command -v lcov &> /dev/null && ! command -v gcovr &> /dev/null; then
        missing_tools+=("lcov or gcovr")
    fi
    
    if [ ${#missing_tools[@]} -ne 0 ]; then
        echo -e "${RED}Error: Missing required tools:${NC}"
        for tool in "${missing_tools[@]}"; do
            echo "  - $tool"
        done
        echo ""
        echo "Install with:"
        echo "  sudo apt install lcov gcovr"
        exit 1
    fi
}

setup_coverage() {
    echo -e "${BLUE}Setting up coverage build...${NC}"
    
    check_tools
    
    mkdir -p "$FULL_BUILD_DIR"
    cd "$FULL_BUILD_DIR"
    
    echo -e "${YELLOW}Configuring CMake with coverage enabled...${NC}"
    cmake -DCMAKE_BUILD_TYPE=Debug \
          -DENABLE_COVERAGE=ON \
          -DBUILD_TESTING=ON \
          -DNO_SANITIZER=ON \
          ..
    
    echo -e "${YELLOW}Building project...${NC}"
    cmake --build . -j$(nproc)
    
    echo -e "${GREEN}Coverage build setup complete!${NC}"
    echo -e "Build directory: ${FULL_BUILD_DIR}"
}

ensure_build() {
    if [ ! -d "$FULL_BUILD_DIR" ] || [ ! -f "$FULL_BUILD_DIR/CMakeCache.txt" ]; then
        echo -e "${YELLOW}Coverage build not found. Setting up...${NC}"
        setup_coverage
    fi
}

run_coverage_html() {
    ensure_build
    echo -e "${BLUE}Generating HTML coverage report...${NC}"
    cd "$FULL_BUILD_DIR"
    cmake --build . --target coverage_gcovr_html_TestVelyraCore
    echo -e "${GREEN}HTML report generated!${NC}"
    echo -e "Location: ${FULL_BUILD_DIR}/coverage_gcovr_TestVelyraCore/index.html"
}

run_coverage_summary() {
    ensure_build
    echo -e "${BLUE}Generating coverage summary...${NC}"
    cd "$FULL_BUILD_DIR"
    cmake --build . --target coverage_summary_TestVelyraCore
}

run_coverage_detailed() {
    ensure_build
    echo -e "${BLUE}Generating detailed coverage report...${NC}"
    cd "$FULL_BUILD_DIR"
    cmake --build . --target coverage_report_TestVelyraCore
}

run_coverage_xml() {
    ensure_build
    echo -e "${BLUE}Generating XML coverage report...${NC}"
    cd "$FULL_BUILD_DIR"
    cmake --build . --target coverage_xml_TestVelyraCore
    echo -e "${GREEN}XML report generated!${NC}"
    echo -e "Location: ${FULL_BUILD_DIR}/coverage_TestVelyraCore.xml"
}

run_coverage_json() {
    ensure_build
    echo -e "${BLUE}Generating JSON coverage report...${NC}"
    cd "$FULL_BUILD_DIR"
    cmake --build . --target coverage_json_TestVelyraCore
    echo -e "${GREEN}JSON report generated!${NC}"
    echo -e "Location: ${FULL_BUILD_DIR}/coverage_TestVelyraCore.json"
}

clean_coverage() {
    ensure_build
    echo -e "${BLUE}Cleaning coverage data...${NC}"
    cd "$FULL_BUILD_DIR"
    cmake --build . --target coverage_clean_TestVelyraCore
    echo -e "${GREEN}Coverage data cleaned!${NC}"
}

view_coverage() {
    run_coverage_html
    
    HTML_FILE="$FULL_BUILD_DIR/coverage_gcovr_TestVelyraCore/index.html"
    
    if [ -f "$HTML_FILE" ]; then
        echo -e "${BLUE}Opening coverage report in browser...${NC}"
        if command -v xdg-open &> /dev/null; then
            xdg-open "$HTML_FILE"
        elif command -v open &> /dev/null; then
            open "$HTML_FILE"
        else
            echo -e "${YELLOW}Could not open browser automatically.${NC}"
            echo -e "Please open: ${HTML_FILE}"
        fi
    else
        echo -e "${RED}Error: HTML report not found at ${HTML_FILE}${NC}"
        exit 1
    fi
}

run_all_reports() {
    ensure_build
    echo -e "${BLUE}Generating all coverage reports...${NC}"
    
    run_coverage_html
    run_coverage_xml
    run_coverage_json
    echo ""
    run_coverage_summary
    
    echo -e "${GREEN}All reports generated!${NC}"
}

# Parse arguments
COMMAND="${1:-html}"

case "$COMMAND" in
    setup)
        setup_coverage
        ;;
    html)
        run_coverage_html
        ;;
    summary)
        run_coverage_summary
        ;;
    detailed)
        run_coverage_detailed
        ;;
    xml)
        run_coverage_xml
        ;;
    json)
        run_coverage_json
        ;;
    clean)
        clean_coverage
        ;;
    view)
        view_coverage
        ;;
    all)
        run_all_reports
        ;;
    help|--help|-h)
        print_usage
        ;;
    *)
        echo -e "${RED}Error: Unknown command '$COMMAND'${NC}"
        echo ""
        print_usage
        exit 1
        ;;
esac
