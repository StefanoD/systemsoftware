#!/bin/sh



echo "Content-type: text/html"
echo ""
echo '<!DOCTYPE html>'
echo '<head>'
echo '<title>CPU Information</title>'
echo '<link rel="stylesheet" href="../style.css"/>'
echo '</head>' 
echo '<body>'
echo '<div>'
echo "<h1>Cpu Information: /proc/cpuinfo</h1>"
echo '<p>'
tail -n 1000 /proc/cpuinfo | sed ':a;N;$!ba;s/\n/\<br\>/g'
echo '</p>'
echo '</div>'
echo '</body>'
echo '</html>'
exit 0
