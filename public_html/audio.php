<?php
$target_dir = "audioFile/"; // GANTI DENGAN DIREKTORI PENYIMPANAN YANG SESUAI
$audioFileType = strtolower(pathinfo($_FILES["audio"]["name"], PATHINFO_EXTENSION));

// Check if audio file is an actual audio file
if(isset($_POST["submit"])) {
  $check = mime_content_type($_FILES["audio"]["tmp_name"]);
  if(strpos($check, 'audio') !== false) {
    echo "File is an audio file - " . $check . ".";
    $uploadOk = 1;
  } else {
    echo "File is not an audio file.";
    $uploadOk = 0;
  }
}

// Check if file already exists
if (file_exists($target_dir . $_FILES["audio"]["name"])) {
  echo "Sorry, file already exists.";
  $uploadOk = 0;
}

// Check file size (adjust as needed)
$maxFileSize = 10000000; // 10MB (in bytes)
if ($_FILES["audio"]["size"] > $maxFileSize) {
  echo "Sorry, your file is too large.";
  $uploadOk = 0;
}

// Allow certain audio file formats
$allowedAudioFormats = array("mp3", "wav", "ogg"); // Add more audio formats if needed
if(!in_array($audioFileType, $allowedAudioFormats)) {
  echo "Sorry, only MP3, WAV, and OGG files are allowed.";
  $uploadOk = 0;
}

if ($uploadOk == 0) {
  echo "Sorry, your file was not uploaded.";
} else {
  if (move_uploaded_file($_FILES["audio"]["tmp_name"], $target_dir . $_FILES["audio"]["name"])) {
    echo "The file ". basename( $_FILES["audio"]["name"]). " has been uploaded.";
  } else {
    echo "Sorry, there was an error uploading your file.";
  }
}
?>
