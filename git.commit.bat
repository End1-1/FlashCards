c:\Projects\Cafe5\mariadb-dump.exe -uroot -posyolia flashcards> c:\Projects\FlashCArds\db.cafe5.sql
del c:\projects\FlashCards\db.cafe5.rar
rar a -df -ma5 -m5 -ep c:\Projects\FlashCards\db.cafe5.rar c:\Projects\FlashCards\db.cafe5.sql
git add .
git commit
git push

pause
