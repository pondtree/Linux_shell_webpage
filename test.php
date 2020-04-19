<html>
<head>
	<title> Team 4 Project </title>
	<body bgcolor="purple">
	<font color="white">
<body>
	<form method="post">
	<h1> Team 4 Project <h1>
	<h2> ls, mkdir, rm, pwd, cd, kill    &&    cp, mv, tar <h2>
	<h3> <h3>
	<mark> Command : <input type="text" name="command"/></mark>
	
	<p> Additional command of ls : <input type="text" name="command2"/></p>
	<p> <input type="submit"/> </p>

	<?php
		$command=($_POST['command']);
		$command2=($_POST['command2']);
		$command3=($_POST['command3']);
		if(strcmp($command,"ls")==0)
			$result=shell_exec("./{$command} {$command2}");
		else
			$result=shell_exec("./test {$command}");
		echo $result;
	?>
</body>
</html>
