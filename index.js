var sys = require('sys')
var exec = require('child_process').exec;
function puts(error, stdout, stderr) { sys.puts(stdout) }

var mysql = require('mysql');
var connection = mysql.createConnection({
	host: 'localhost',
	user: 'arduino',
	password: 'arduino1234',
	database: 'arduino_temp'
});
connection.connect();

var serialport = require("serialport"); 
var SerialPort = serialport.SerialPort;
var _ = require("underscore");

var serialPort = new SerialPort("/dev/ttyACM0", {
  baudrate: 9600,
  parser: serialport.parsers.readline("\n")
});

serialPort.on("open", function () {
  console.log('open');
  serialPort.on('data', onDataChange);
});

var recordTemperatureThrottled = _.throttle(recordTemperature, 600000);
var triggerActionDebounced = _.debounce(triggerAction, 5000);

function onDataChange (data) {
	try {
		var jsonData = JSON.parse(data);
		processJsonData(jsonData);
	} catch (e) {
		console.log('not a valid json');
	}
};

function processJsonData (jsonData) {
	switch (jsonData.type) {
		case "temperature":
			recordTemperatureThrottled(jsonData.value);
			break;
		case "button":
			triggerActionDebounced();
			break;
	}
}

function recordTemperature (temperature) {
	connection.query('INSERT INTO `temperature`(`value`) VALUES (' + temperature + ')', function(err, rows, fields) {
		if (err) throw err;
	});
	console.log('recording temperature', temperature);
}

function triggerAction () {
	exec("service plexmediaserver restart", puts);
}

process.on('exit', function() { 
	console.log('exiting');
	connection.end(); 
});