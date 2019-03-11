const fs = require('fs');
const https = require('https');

const options = {
  hostname: 'localhost',
  port: 3200,
  path: '/works',
  method: 'GET',
  key: fs.readFileSync('ssl/client1-key.pem'),
  cert: fs.readFileSync('ssl/client1-crt.pem'),
  ca: fs.readFileSync('ssl/ca-crt.pem'),
};

const req = https.request(options, (res) => {
  res.on('data', (data) => {
    process.stdout.write(data);
  });
});

req.end();

req.on('error', (e) => {
  console.error(e);
});

const options2 = {
  hostname: 'localhost',
  port: 3200,
  path: '/breaks',
  method: 'GET',
  key: fs.readFileSync('ssl/client2-key.pem'),
  cert: fs.readFileSync('ssl/client2-crt.pem'),
  ca: fs.readFileSync('ssl/ca-crt.pem'),
};

const req2 = https.request(options2, (res) => {
  res.on('data', (data) => {
    process.stdout.write(data);
  });
});

req2.end();

req2.on('error', (e) => {
  console.error(e);
});
