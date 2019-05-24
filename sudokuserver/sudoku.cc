#include <node.h>
#include <vector>
#include <time.h>
#include "sudoku.c"

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Object;
using v8::String;
using v8::Array;
using v8::Integer;
using v8::Value;

void GenerateSudokuMethod(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    int hints = args[0].As<Integer>()->Value();
    int maxtries = args[1].As<Integer>()->Value();
    
    srand(clock());

    Cell* solution = (Cell*) malloc(sizeof(Cell) * SUDOKU_SIZE * SUDOKU_SIZE);
    Cell* sudoku = generateSudoku(hints, maxtries, solution);

    Local<Object> results = Object::New(isolate);

    Local<Array> sudokuArray = Array::New(isolate);
    Local<Array> solutionArray = Array::New(isolate);

    for(int i = 0; i < SUDOKU_SIZE * SUDOKU_SIZE; i++) {
        Local<Integer> obj = Integer::New(isolate, sudoku[i].value);
        sudokuArray->Set(i, obj);
    }

    free(sudoku);

    for(int i = 0; i < SUDOKU_SIZE * SUDOKU_SIZE; i++) {
        Local<Integer> obj = Integer::New(isolate, solution[i].value);
        solutionArray->Set(i, obj);
    }

    free(solution);

    results->Set(String::NewFromUtf8(isolate, "sudoku"), sudokuArray);
    results->Set(String::NewFromUtf8(isolate, "solution"), solutionArray);

    args.GetReturnValue().Set(results);
}

void SolveSudokuMethod(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Cell* grid = createBoard();
    for(int i = 0; i < SUDOKU_SIZE * SUDOKU_SIZE; i++) {
        (grid + i)->value = args[0].As<Array>()->Get(i).As<Integer>()->Value();
    }
    Cell* solution = solveGrid(grid);

    free(grid);

    Local<Array> solutionArray = Array::New(isolate);
    for(int i = 0; i < SUDOKU_SIZE * SUDOKU_SIZE; i++) {
        Local<Integer> obj = Integer::New(isolate, solution[i].value);
        solutionArray->Set(i, obj);
    }

    free(solution);

    args.GetReturnValue().Set(solutionArray);
}

void Initialize(Local<Object> exports) {
    NODE_SET_METHOD(exports, "generate", GenerateSudokuMethod);
    NODE_SET_METHOD(exports, "solve", SolveSudokuMethod);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)