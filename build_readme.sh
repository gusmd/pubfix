cp README.md README.tmp
echo -e "\n# License\n" >> README.tmp
cat LICENSE >> README.tmp
echo -e "\n## 3rd Party\n" >> README.tmp
echo -e "Parts of this are subject to their own license terms." >> README.tmp
echo -e "\n### QHotkey\n" >> README.tmp
cat QHotkey/LICENSE >> README.tmp
