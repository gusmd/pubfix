tmp="README.tmp"
cp README.md $tmp
echo -e "\n# License\n" >> $tmp
cat LICENSE >> $tmp

echo -e "\n## 3rd Party\n" >> $tmp
echo -e "Parts of this are subject to their own license terms." >> $tmp

echo -e "\n### QHotkey\n" >> $tmp
echo -e "\`\`\`" >> $tmp
cat QHotkey/LICENSE >> $tmp
echo -e "\`\`\`" >> $tmp

echo -e "\n### QDarkStyleSheet\n" >> $tmp
echo -e "\`\`\`" >> $tmp
cat QDarkStyleSheet/LICENSE.md >> $tmp
echo -e "\`\`\`" >> $tmp
