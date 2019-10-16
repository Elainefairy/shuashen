
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
   
   
   
   
   
    $sql =<<<EOF
      UPDATE THEDATAS set NUMBER =$_POST["NUMBER"],STATE=$_POST["STATE"],RESULT=$_POST["RESULT"],OLDPHOTO$_POST["OLDPHOTO"],NEWPHOTO=$_POST["NEWPHOTO"] where ID=$GET['ID'];
EOF;
   $ret = $db->exec($sql);
   if(!$ret){
      echo $db->lastErrorMsg();
   } else {
      echo $db->changes(), " Record updated successfully\n";
   }

   $db->close();
   
  

?>