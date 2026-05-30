import xml.etree.ElementTree as ET
import sys
import os
from datetime import datetime

def parse_results(xml_path):
    tree = ET.parse(xml_path)
    root = tree.getroot()

    suites = []
    for suite in root.findall('testsuite'):
        tests = []
        for test in suite.findall('testcase'):
            failure = test.find('failure')
            tests.append({
                'name': test.get('name'),
                'status': 'FAIL' if failure is not None else 'PASS',
                'message': failure.text if failure is not None else ''
            })
        suites.append({
            'name': suite.get('name'),
            'tests': tests,
            'total': int(suite.get('tests', 0)),
            'failures': int(suite.get('failures', 0)),
            'time': suite.get('time', '0')
        })
    return suites

def generate_html(suites, output_path):
    total_tests = sum(s['total'] for s in suites)
    total_failures = sum(s['failures'] for s in suites)
    overall_pass = total_failures == 0
    timestamp = datetime.utcnow().strftime('%Y-%m-%d %H:%M UTC')

    status_color = '#2ecc71' if overall_pass else '#e74c3c'
    status_text = 'ALL TESTS PASSED' if overall_pass else f'{total_failures} TEST(S) FAILED'

    suite_html = ''
    for suite in suites:
        suite_pass = suite['failures'] == 0
        suite_color = '#2ecc71' if suite_pass else '#e74c3c'
        suite_indicator = '✓' if suite_pass else '✗'

        test_rows = ''
        for test in suite['tests']:
            row_color = '#2ecc71' if test['status'] == 'PASS' else '#e74c3c'
            indicator = '✓' if test['status'] == 'PASS' else '✗'
            message = f'<div class="failure-msg">{test["message"]}</div>' if test['message'] else ''
            test_rows += f'''
            <tr>
                <td><span style="color:{row_color}">{indicator}</span> {test["name"]}</td>
                <td style="color:{row_color}">{test["status"]}</td>
                <td>{message}</td>
            </tr>'''

        suite_html += f'''
        <div class="suite">
            <div class="suite-header" style="border-left: 4px solid {suite_color}">
                <span style="color:{suite_color}">{suite_indicator}</span>
                <strong>{suite["name"]}</strong>
                <span class="suite-stats">{suite["total"] - suite["failures"]}/{suite["total"]} passed — {suite["time"]}s</span>
            </div>
            <table>
                <thead><tr><th>Test</th><th>Status</th><th>Message</th></tr></thead>
                <tbody>{test_rows}</tbody>
            </table>
        </div>'''

    html = f'''<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Automotive Digital Twin — CI Dashboard</title>
    <style>
        body {{ font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif; background: #1a1a2e; color: #eee; margin: 0; padding: 20px; }}
        .header {{ text-align: center; padding: 30px 0; }}
        .header h1 {{ font-size: 1.8em; margin: 0; color: #a0c4ff; }}
        .header p {{ color: #888; margin: 8px 0 0; }}
        .overall {{ text-align: center; padding: 20px; margin: 20px auto; max-width: 400px; border-radius: 8px; background: {status_color}22; border: 2px solid {status_color}; }}
        .overall h2 {{ margin: 0; color: {status_color}; font-size: 1.4em; }}
        .overall p {{ margin: 6px 0 0; color: #aaa; }}
        .suite {{ background: #16213e; border-radius: 8px; margin: 20px auto; max-width: 900px; overflow: hidden; }}
        .suite-header {{ padding: 14px 20px; display: flex; align-items: center; gap: 12px; background: #0f3460; }}
        .suite-stats {{ margin-left: auto; color: #888; font-size: 0.9em; }}
        table {{ width: 100%; border-collapse: collapse; }}
        th {{ padding: 10px 20px; text-align: left; background: #0a2540; color: #888; font-size: 0.85em; }}
        td {{ padding: 10px 20px; border-top: 1px solid #ffffff11; font-size: 0.9em; }}
        .failure-msg {{ color: #e74c3c; font-family: monospace; font-size: 0.8em; margin-top: 4px; white-space: pre-wrap; }}
        .timestamp {{ text-align: center; color: #555; font-size: 0.8em; margin-top: 40px; }}
    </style>
</head>
<body>
    <div class="header">
        <h1>Automotive Digital Twin</h1>
        <p>CI Test Dashboard</p>
    </div>
    <div class="overall">
        <h2>{status_text}</h2>
        <p>{total_tests - total_failures} / {total_tests} tests passing</p>
    </div>
    {suite_html}
    <div class="timestamp">Last updated: {timestamp}</div>
</body>
</html>'''

    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    with open(output_path, 'w') as f:
        f.write(html)
    print(f"Dashboard written to {output_path}")

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: generate_dashboard.py <xml_input> <html_output>")
        sys.exit(1)
    suites = parse_results(sys.argv[1])
    generate_html(suites, sys.argv[2])