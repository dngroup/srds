const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const fse = require('fs-extra');
const shell = require('shelljs');
const app = express();
const jsonParser = bodyParser.json();

app.use(express.static('public'));
app.use(cors());

const start = Date.now();

shell.mkdir('-p', '/home/pi/logs/' + start);

async function onSetWord(req, res) {
  const routeParams = req.params;
  const word = routeParams.word;
  await fse.writeJson('/home/pi/logs/' + start + '/' + routeParams.word + '.json', req.body);
  res.json({success: true});
}
app.post('/set/:word', jsonParser, onSetWord);

app.listen(9999, function () {
  console.log('Server listening on port 9999');
});

