const express = require('express');
const app = express()

app.use(express.static('public'))// used to server the static file in public

app.listen(2222,()=> console.log("Server is running at port 2222"));

