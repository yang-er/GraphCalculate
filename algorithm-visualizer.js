const { Tracer, Array2DTracer, Array1DTracer, GraphTracer, LogTracer, Layout, VerticalLayout, HorizontalLayout } = require('algorithm-visualizer');

const tracer = new GraphTracer("Graph").directed(true);
const tracerS = new Array2DTracer("Shortest");
const tracerSum = new Array1DTracer("SP Sum");
const tracerAvg = new Array1DTracer("Average")
const vv = new VerticalLayout([tracerSum, tracerAvg]);
const hh = new HorizontalLayout([tracer, tracerS, vv]);
const logger = new LogTracer();
Layout.setRoot(new VerticalLayout([hh, logger]));
tracer.log(logger);

const G = [
    [0, 1, 0, 0, 1],
    [0, 0, 0, 0, 0],
    [1, 1, 0, 1, 0],
    [0, 0, 0, 0, 0],
    [1, 0, 1, 0, 0]
];
tracer.set(G);

const MAX_VALUE = Infinity;
const S = []; // S[end] returns the distance from start node to end node
const SUM = [];
const AVG = [];

for (let i = 0; i < G.length; i++) {
    S[i] = [];
    SUM[i] = Infinity;
    AVG[i] = Infinity;
    for (let j = 0; j < G.length; j++)
        S[i][j] = MAX_VALUE;
}

tracerS.set(S);
tracerSum.set(SUM);
tracerAvg.set(AVG);
Tracer.delay();

function Dijkstra(start, end) {
    let minIndex;
    let minDistance;
    const D = []; // D[i] indicates whether the i-th node is discovered or not
    for (let i = 0; i < G.length; i++) D.push(false);
    S[start] = 0; // Starting node is at distance 0 from itself
    tracerS.patch(start, start, S[start]);
    Tracer.delay();
    tracerS.depatch(start, start);
    tracerS.select(start, start);
    let k = G.length;
    while (k--) {
        // Finding a node with the shortest distance from S[minIndex]
        minDistance = MAX_VALUE;
        for (let i = 0; i < G.length; i++) {
            if (S[i] < minDistance && !D[i]) {
                minDistance = S[i];
                minIndex = i;
            }
        }
        if (minDistance === MAX_VALUE) break; // If there is no edge from current node, jump out of loop
        D[minIndex] = true;
        tracerS.select(start, minIndex);
        tracer.visit(minIndex);
        //Tracer.delay();
        // For every unvisited neighbour of current node, we check
        // whether the path to it is shorter if going over the current node
        for (let i = 0; i < G.length; i++) {
            if (G[minIndex][i] && S[i] > S[minIndex] + G[minIndex][i]) {
                S[i] = S[minIndex] + G[minIndex][i];
                tracerS.patch(start, i, S[i]);
                tracer.visit(i, minIndex, S[i]);
                Tracer.delay();
                tracerS.depatch(start, i);
                tracer.leave(i, minIndex);
            }
        }
        tracer.leave(minIndex);
        Tracer.delay();
    }
    
    if (S[end] === MAX_VALUE) {
        logger.println(`there is no path from ${start} to ${end}`);
    }
}

function ClosenessCentrality() {
    for (let i = 0; i < G.length; i++) {
        for (let j = 0; j < G.length; j++)
            S[j] = Infinity;
        let sum=0;
        Dijkstra(i, MAX_VALUE);
        for (let j = 0; j < G.length; j++)
            sum = sum + S[j];
        SUM[i] = sum;
        AVG[i] = G.length / sum;
        tracerSum.patch(i, sum);
        tracerAvg.patch(i, AVG[i]);
        Tracer.delay();
        logger.println(`${AVG[i]}`);
        tracerSum.depatch(i);
        tracerAvg.depatch(i);
    }
    
    let id=0;
    let max=0;
    for (let i = 0;i <= G.length; i++) {
        if (AVG[i] >= max){
            id = i;
            max = AVG[i];
        }
    }
    
    logger.println(`the closecentre point is ${id}`);
    logger.println(`the closecentre value is ${max}`);
    tracerSum.select(id);
    tracerAvg.select(id);
    tracer.select(id);
    for (let i = 0; i < G.length; i++) {
        if (G[id][i] > 0) {
            tracer.select(i, id);
            tracer.deselect(i);
        }
        
        if (G[i][id] > 0) {
            tracer.select(id, i);
            tracer.deselect(id);
        }
    }
}

ClosenessCentrality();
