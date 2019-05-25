const sudoku = require("./build/Release/sudoku_native");
const http = require("http");
const url = require("url");

const server = http.createServer((req, res) => {
    let urlparse = url.parse(req.url, true);
    if (urlparse.pathname == "/") {
        let params = urlparse.query;
        let start = new Date().getTime();
        let clues = 0;
        if ("clues" in params) {
            clues = Number(params.clues);
            if (clues < 23) clues = 23;
            if (clues > 81) clues = 81;
        } else {
            clues = 30;
        }
        console.log("Attempting to generate sudoku with", clues, "clues.");
        res.writeHead(200, { 'Content-Type': 'application/json' });
        let resultObject = sudoku.generate(clues, 300);

        var count = 0;
        for (var i = 0; i < 81; ++i) {
            if (resultObject.sudoku[i] != 0)
                count++;
        }
        resultObject.clues = count;

        res.write(JSON.stringify(resultObject));
        console.log("Generated sudoku with", count, "clues in", new Date().getTime() - start, "ms.");
    }
    res.end();
});

server.listen(80, "0.0.0.0", () => {
    console.log("Started server on port 80")
});
