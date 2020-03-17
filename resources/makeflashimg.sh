#!/bin/sh

#TP="/media/floppy"
TP="/tmp/casefile"

mkdir $TP

rm CaseFile.fat
mkfs.fat -C -S 512 -M 0xF8 -r 112 -D 0 -n "CASE FILE" CaseFile.fat 256
mount -t msdos -o loop CaseFile.fat $TP

#dd if=/dev/zero bs=1k count=256 of=CaseFile.fat
#newfs_msdos -F 12 -S 512 -e 112 -v "CASE FILE" -m 0xF8 ./CaseFile.fat
#hdiutil attach CaseFile.fat
#minimodem --rx rtty -f test.ogg


cp readme.md $TP/README.md
python memail.py ph4n70m.eml
cp ph4n70m.eml $TP/
cp puzzle.txt $TP/
#cat rtty.txt | minimodem --tx rtty -f rtty.ogg
cp rtty.ogg $TP/
cp sample.cap $TP/sample.cap
cp blackmail.rtf $TP/blckmail.rtf
rm Chess.jpg
steghide embed -f -cf Chess_clean.jpg -ef msg.txt -p checkmate -sf Chess.jpg
cp Chess.jpg $TP/chess.jpg
cp recover.pdf $TP/
#unlink $TP/removal.pdf
df $TP
umount $TP

rm -rf $TP

python makedialogs.py
cat dialogs.bin >> CaseFile.fat

convert bsides2020.png -colorspace gray +matte -colors 2 -depth 1 temp.gray
truncate -s +50 temp.gray
cat temp.gray >> CaseFile.fat

for i in boss1 detectiveguy1 detectivegirl1 enemy1 girl1 itguy1 worker1 comp1 lab1 roomnoc roomfront roomyard roomit roombreak roommeet roomback roomtunnel roomstorage roomsecret roomfront2 roomcage roomfire roomyard2 roomstorage2 virus ending
do
  convert $i.png -colorspace gray +matte -colors 2 -depth 1 temp.gray
  truncate -s +10 temp.gray
  cat temp.gray >> CaseFile.fat
done

convert pow.png -colorspace gray +matte -colors 2 -depth 1 temp.gray
truncate -s +184 temp.gray
cat temp.gray >> CaseFile.fat

truncate -s +256 CaseFile.fat

rm temp.gray

ls -lsah CaseFile.fat

