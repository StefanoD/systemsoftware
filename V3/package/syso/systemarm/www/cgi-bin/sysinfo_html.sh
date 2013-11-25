#!/bin/sh

echo "Content-type: text/html"
echo ""

echo '<!DOCTYPE html>'
echo '<head>'
echo '<title>System Informations</title>'
echo '<link rel="stylesheet" href="../style.css"/>'
echo '</head>' 
echo '<body>'
echo '<div>'
echo '<h1>Systeminformationen</h1>'
./sysinfo_html
echo '</div>'
echo '</body>'
echo '</html>'
exit 0
