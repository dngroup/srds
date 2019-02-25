const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const app = express();
const jsonParser = bodyParser.json();

app.use(express.static('public'));
app.use(cors());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

var routes = require("./routes.js")(app);

app.listen(8888, function () {
  console.log('Tracker listening on port 8888');
});

