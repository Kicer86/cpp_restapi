#!/usr/bin/env bash
# Generates the landing page (index.html) for versioned documentation.
# Run from within the gh-pages directory.
set -euo pipefail

VERSIONS=$(find . -maxdepth 1 -type d -name 'v*' | sed 's|./||' | sort -rV)
LATEST=$(echo "$VERSIONS" | head -n1)

cat > index.html << 'EOF'
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>cpp Rest API — Documentation</title>
  <style>
    :root {
      --primary: #2980b9;
      --primary-dark: #1a5276;
      --bg: #f8f9fa;
      --card-bg: #ffffff;
      --text: #2c3e50;
      --text-secondary: #6c757d;
      --border: #e9ecef;
      --radius: 10px;
    }
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
      background: var(--bg);
      color: var(--text);
      min-height: 100vh;
      display: flex;
      flex-direction: column;
    }
    .hero {
      background: linear-gradient(135deg, var(--primary) 0%, var(--primary-dark) 100%);
      color: #fff;
      padding: 60px 20px 50px;
      text-align: center;
    }
    .hero h1 {
      font-size: 2.2rem;
      font-weight: 700;
      margin-bottom: 12px;
    }
    .hero p {
      font-size: 1.1rem;
      opacity: 0.9;
      max-width: 600px;
      margin: 0 auto;
    }
    .content {
      max-width: 720px;
      margin: -30px auto 0;
      padding: 0 20px 60px;
      flex: 1;
      width: 100%;
    }
    .quick-access {
      background: var(--card-bg);
      border-radius: var(--radius);
      box-shadow: 0 2px 12px rgba(0,0,0,0.08);
      padding: 28px 32px;
      margin-bottom: 32px;
      display: flex;
      align-items: center;
      gap: 16px;
      flex-wrap: wrap;
    }
    .quick-access label {
      font-weight: 600;
      font-size: 1rem;
      white-space: nowrap;
    }
    .quick-access select {
      flex: 1;
      min-width: 180px;
      padding: 10px 14px;
      font-size: 1rem;
      border: 2px solid var(--border);
      border-radius: 6px;
      background: var(--bg);
      color: var(--text);
      cursor: pointer;
      transition: border-color 0.2s;
    }
    .quick-access select:focus {
      outline: none;
      border-color: var(--primary);
    }
    .quick-access .btn {
      display: inline-block;
      padding: 10px 24px;
      background: var(--primary);
      color: #fff;
      border: none;
      border-radius: 6px;
      font-size: 1rem;
      font-weight: 600;
      cursor: pointer;
      text-decoration: none;
      transition: background 0.2s;
    }
    .quick-access .btn:hover {
      background: var(--primary-dark);
    }
    .section-title {
      font-size: 1.1rem;
      font-weight: 600;
      margin-bottom: 16px;
      color: var(--text-secondary);
      text-transform: uppercase;
      letter-spacing: 0.5px;
    }
    .version-grid {
      display: grid;
      grid-template-columns: repeat(auto-fill, minmax(200px, 1fr));
      gap: 12px;
    }
    .version-card {
      background: var(--card-bg);
      border: 1px solid var(--border);
      border-radius: 8px;
      padding: 16px 20px;
      text-decoration: none;
      color: var(--text);
      transition: box-shadow 0.2s, border-color 0.2s;
      display: flex;
      align-items: center;
      gap: 10px;
    }
    .version-card:hover {
      box-shadow: 0 2px 8px rgba(0,0,0,0.1);
      border-color: var(--primary);
    }
    .version-card .name {
      font-weight: 600;
      font-size: 1.05rem;
    }
    .version-card .badge {
      font-size: 0.7rem;
      padding: 2px 8px;
      border-radius: 10px;
      font-weight: 600;
      text-transform: uppercase;
      letter-spacing: 0.3px;
      white-space: nowrap;
    }
    .badge-dev {
      background: #e8f5e9;
      color: #2e7d32;
    }
    .badge-latest {
      background: #e3f2fd;
      color: #1565c0;
    }
    footer {
      text-align: center;
      padding: 20px;
      color: var(--text-secondary);
      font-size: 0.85rem;
      border-top: 1px solid var(--border);
    }
    footer a { color: var(--primary); text-decoration: none; }
    footer a:hover { text-decoration: underline; }
  </style>
</head>
<body>
  <div class="hero">
    <h1>cpp Rest API</h1>
    <p>C++ library for REST API access with Qt, Curl and cpp-httplib backends</p>
  </div>
  <div class="content">
    <div class="quick-access">
      <label for="version-select">Go to docs:</label>
      <select id="version-select">
EOF

# Add master option
echo '        <option value="master">master (development)</option>' >> index.html

# Add version options
FIRST=true
for V in $VERSIONS; do
  LABEL="$V"
  if $FIRST; then LABEL="$V (latest release)"; FIRST=false; fi
  echo "        <option value=\"${V}\">${LABEL}</option>" >> index.html
done

cat >> index.html << 'EOF'
      </select>
      <button class="btn" onclick="go()">Open →</button>
    </div>
    <div class="section-title">All versions</div>
    <div class="version-grid">
      <a class="version-card" href="master/">
        <span class="name">master</span>
        <span class="badge badge-dev">dev</span>
      </a>
EOF

# Add version cards
FIRST=true
for V in $VERSIONS; do
  BADGE=""
  if $FIRST; then BADGE='<span class="badge badge-latest">latest</span>'; FIRST=false; fi
  echo "      <a class=\"version-card\" href=\"${V}/\"><span class=\"name\">${V}</span>${BADGE}</a>" >> index.html
done

cat >> index.html << 'EOF'
    </div>
  </div>
  <footer>
    Generated with <a href="https://www.doxygen.nl/">Doxygen</a> +
    <a href="https://github.com/jothepro/doxygen-awesome-css">doxygen-awesome</a>
  </footer>
  <script>
    function go() {
      var v = document.getElementById('version-select').value;
      window.location.href = v + '/';
    }
    document.getElementById('version-select').addEventListener('keydown', function(e) {
      if (e.key === 'Enter') go();
    });
  </script>
</body>
</html>
EOF
