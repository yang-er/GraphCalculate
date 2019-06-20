const { Tracer, GraphTracer, LogTracer, Layout, VerticalLayout } = require('algorithm-visualizer');

const N = 10;
let M = Math.floor(Math.pow(N, 0.75));
const tracer = new GraphTracer().directed(true);
const logger = new LogTracer();
Layout.setRoot(new VerticalLayout([tracer, logger]));

const MAX_VALUE = Infinity;
const S = [];
for (let i = 0; i <= N; i++) S[i] = 0;

tracer.addNode(1);
logger.println(`${N} ${N+M}`);
Tracer.delay();

let number_tot = 0;

function random_walk(depth, deg)
{
    if (depth === 0 && number_tot < N) return true;
    let p = 50.0 / (100 + deg * 30 + depth);
    let now = Math.random();
    return now > p;
}

function random_int() {
    return Math.floor(Math.random() * 32768);
}

function dfs(current, depth) {
    while (number_tot < N) {
        if (random_walk(depth, S[current])) {
            number_tot++;
            let ccc = number_tot;
            tracer.addNode(ccc);
            Tracer.delay();
            tracer.addEdge(current, ccc);
            tracer.visit(ccc, current);
            logger.println(`${current} ${ccc}`);
            Tracer.delay();
            S[current]++;
            dfs(ccc, depth+1);
            tracer.leave(ccc, current);
            Tracer.delay();
        } else {
            return;
        }
    }
}

tracer.addNode(1);
tracer.visit(1);
Tracer.delay();
dfs(1, 0);
tracer.leave(1);
Tracer.delay();

for (let i = 0; i < M; i++) {
    let u = random_int() % N + 1;
    let v = u;
    while (v === u) v = random_int() % N + 1;
    tracer.select(u);
    Tracer.delay();
    tracer.addEdge(u, v);
    tracer.visit(v, u);
    Tracer.delay();
    tracer.leave(v, u);
    tracer.deselect(u);
    logger.println(`${u} ${v}`);
    Tracer.delay();
}
