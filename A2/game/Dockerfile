FROM node:19.4-slim as base

WORKDIR /game

EXPOSE 8080

COPY . /game

RUN npm i

CMD [ "npm", "run", "start" ]