FROM node:12.3.1

WORKDIR /app

COPY sudokuserver /app

RUN npm i --save node-gyp

RUN node ./node_modules/node-gyp/bin/node-gyp.js configure 
RUN node ./node_modules/node-gyp/bin/node-gyp.js build 

CMD ["node", "index.js"]