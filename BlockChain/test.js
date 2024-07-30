const Blockchain = require("./blockchain");

let aliencoin = new Blockchain();

aliencoin.createNewTransaction(
    "100",
    "0xalien",
    "0xterrestrial"
)

aliencoin.createNewBlock();

console.log(aliencoin);