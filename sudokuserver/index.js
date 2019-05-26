const sudoku = require("./build/Release/sudoku_native");
const express = require("express");
const cors = require("cors");

function chunkArray(arr, chunkSize) {
    if(arr.length % chunkSize == 0) {
        let newArray = [];
        let chunkCount = arr.length / chunkSize;
        for(let i = 0; i < chunkCount; i++) {
            newArray.push(arr.slice(i * chunkSize, i * chunkSize + chunkSize));
        }
        return newArray;
    } else {
        throw "array length must be divisible by chunk size";
    }
}

const app = express();
app.use(cors());

app.get("/", (req, res) => {
    let start = new Date().getTime();
    let clues = 0;
    if ("clues" in req.query) {
        clues = Number(req.query.clues);
        if (clues < 23) clues = 23;
        if (clues > 81) clues = 81;
    } else {
        clues = 30;
    }
    console.log("Attempting to generate sudoku with", clues, "clues.");
    res.set('Content-Type', 'application/json');
    let resultObject = sudoku.generate(clues, 300);

    var count = 0;
    for (var i = 0; i < 81; ++i) {
        if (resultObject.sudoku[i] != 0)
            count++;
    }
    resultObject.clues = count;
    resultObject.sudoku = chunkArray(resultObject.sudoku, 9);
    resultObject.solution = chunkArray(resultObject.solution, 9);

    res.send(JSON.stringify(resultObject, null, 4));
    console.log("Generated sudoku with", count, "clues in", new Date().getTime() - start, "ms.");
});

let port = 2012;
app.listen(port, () => {
    console.log("Started server on port " + port);
});