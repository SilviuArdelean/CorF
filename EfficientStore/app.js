
var express = require("express");
var bodyParser = require('body-parser')
var app = express();
app.use(express.static(__dirname + '/'));

//if (process.env.DEBUG) {
//  const addon = require('./cpp/build/Debug/engine_addon');
//} else {
  const addon = require('./cpp/build/Release/engine_addon');
//}

var urlencodedParser = bodyParser.urlencoded({ extended: false })

app.use('/assets', express.static('stuff'));

app.get("/", function (req, res) {
    res.sendFile(__dirname + "/index.html");
});

app.post('/addnewitem', urlencodedParser, function (req, res)  {
	var search_string = req.body.search_text;
	
	console.log("new item to add:"); 
	console.log("{");
	console.log("	person_id: " + req.body.person_id);
	console.log("	name: " + req.body.name);
	console.log("	surname: " + req.body.surname);
	console.log("	email: " + req.body.email);
	console.log("}");

	res.write("new item to add be added:");
	res.write("<br>{");
	res.write("<br>		person_id: " + req.body.person_id);
	res.write("<br>		name: " + req.body.name);
	res.write("<br>		surname: " + req.body.surname);
	res.write("<br>		email: " + req.body.email);
   res.write("<br>}"); 
   res.end();

   addon.addItem(req.body.person_id, req.body.name, req.body.surname, req.body.email);
   
});

app.post('/search', urlencodedParser, function (req, res) {

   var search_string = req.body.search_text;

  var addon_result = addon.effectiveSearch(search_string);

   res.write("The server has received a search request for: { key = " + req.body.key2delete + " } "); 
   res.write("Search request for: " + addon_result);

   res.end();
});

app.post('/deletekey', urlencodedParser, function (req, res)  {
	var search_string = req.body.search_text;
	
	console.log("Delete request=: { key: " + req.body.key2delete + " } "); 

	res.write("The server has received a delete request for : { key = " + req.body.key2delete + " } "); 

	res.end();
});

addon.initEngine();

app.listen(3000); 
