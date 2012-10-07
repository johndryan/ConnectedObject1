<?php
    header("content-type: text/xml");
    echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	$myFile = "data.txt";
	$fh = fopen($myFile, 'w') or die("can't open file");
	$stringData = "~" . trim($_REQUEST['Body']) . "~";;
	fwrite($fh, $stringData);
	fclose($fh);

	$myArchive = "archive.txt";
	$fh = fopen($myArchive, 'a') or die("can't open file");
	$stringData = trim($_REQUEST['Body']) . "\n\n";
	fwrite($fh, $stringData);
	fclose($fh);

    require "twilio-php/Services/Twilio.php";
    $AccountSid = "--TWILIO_ACCOUNT_ID--";
    $AuthToken = "--TWILIO_AUTH_TOKEN--";
    $client = new Services_Twilio($AccountSid, $AuthToken);
?>
<Response>
    <Sms>Thanks for your message. Now sending to screen.</Sms>
</Response>
