#!/bin/sh

tmp_path="/tmp"

echo "Content-type: text/html"
echo ""
echo '<!DOCTYPE html>'
echo '<head>'
echo '<title>System Informations</title>'
echo '<link rel="stylesheet" href="../style.css"/>'
echo '</head>' 
echo '<body>'
echo "<h1>Trying to remove Tempfiles! ( $tmp_path )</h1>"
rm -rf $tmp_path/*
echo '</body>'
echo '</html>'
exit 0

