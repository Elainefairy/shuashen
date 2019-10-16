
<?php
   class MyDB extends SQLite3
   {
      function __construct()
      {
         $this->open('our.db');
      }
   }
   $db = new MyDB();
   if(!$db){
      echo $db->lastErrorMsg();
   } else {
      echo "Opened database successfully\n";
   }
   
   
   
  // $NUMBER=$STATE=$RESULT=$OLDPHOTO=$NEWPHOTO=$ATIME=$STIME="";
   
    $sql =<<<EOF
	INSERT INTO THEDATAS (NUMBER,STATE,RESULT,OLDPHOTO,NEWPHOTO,ATIME) VALUES ($_POST["NUMBER"],$POST["STATE"],$POST["RESULT"],$POST["OLDPHOTO"],$POST["NEWPHOTO"],datatime('now'));

EOF;
   $ret = $db->exec($sql);
   if(!$ret){
      echo $db->lastErrorMsg();
   } else {
      echo $db->changes(), " Record updated successfully\n";
   }

   $db->close();
   
  

?>