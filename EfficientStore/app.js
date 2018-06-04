
var express = require("express");
var bodyParser = require('body-parser')
var app = express();
app.use(express.static(__dirname + '/'));


  const addon = require('./cpp/build/Release/engine_addon');


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

   var add_result = addon.addItem(req.body.person_id, req.body.name, req.body.surname, req.body.email);

   res.write(add_result);
   res.end();   
});

app.post('/search', urlencodedParser, function (req, res) {

   var search_string = req.body.search_text;

   var search_result = addon.effectiveSearch(search_string);

   console.log("Search request for: { key = " + search_string + " } "); 
   console.log("--------------------------------------------------------");
   console.log("Search result: " + search_result);

   res.write(search_result);

   res.end();
});

app.post('/deletekey', urlencodedParser, function (req, res)  {
	
	console.log("Delete request { key: " + req.body.key2delete + " } "); 

   var delete_result = addon.deleteItem(req.body.key2delete);

   res.write(delete_result); 

	res.end();
});

addon.initEngine();

app.listen(3000); 
