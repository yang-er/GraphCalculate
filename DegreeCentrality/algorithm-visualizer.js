const { Tracer, Array1DTracer,Array2DTracer, GraphTracer, LogTracer, Randomize, Layout, VerticalLayout,HorizontalLayout } = require('algorithm-visualizer');

const gg = [
  [0, 0, 1, 1, 0, 0, 0],
  [1, 0, 0, 0, 0, 0, 0],
  [0, 1, 0, 0, 0, 0, 1],
  [0, 1, 0, 1, 0, 0, 0],
  [0, 0, 0, 0, 0, 1, 0],
  [0, 1, 0, 0, 1, 0, 0],
  [0, 0, 0, 0, 1, 0, 0],
];
const gp=[];
const gs = [
  [10,19],
  [0,2],
  [0,3],
  [1,7],
  [1,5],
  [2,6],
  [3,1],
  [4,1],
  [4,5],
  [4,9],
  [5,4],
  [6,4],
  [6,1],
  [7,9],
  [7,8],
  [8,9],
  [8,2],
  [8,1],
  [9,0],
  [9,3],
];


const array2dTracer = new Array2DTracer('AdjacentList');
const tracer = new GraphTracer("Graph").directed(true);
const degree = new Array1DTracer("Degree");
const logger = new LogTracer("Log");
Layout.setRoot(new VerticalLayout([tracer,new HorizontalLayout([new VerticalLayout([degree,logger]),array2dTracer])]));
var n,m;
var maxdegree;
var maxnode;
tracer.log(logger);
array2dTracer.set(gp);
// tracer.set(gg);

const MAX_VALUE = Infinity;
const du = []; 

creatGraph();
array2dTracer.set(gp);

degreeCentrality();
Tracer.delay();

function creatGraph()
{
    n = gs[0][0];
    m = gs[0][1];
    logger.println(n);

    for(let a=0; a<n ; a++)
        gp.push([]);
    for(let a=1 ; a<= m ; a++)
    {
        var p1 = gs[a][0];
        var p2 = gs[a][1];
        tracer.addNode(p1);
        tracer.addNode(p2);
        tracer.addEdge(p1,p2);
        gp[p1].push(p2);
    }
}

function degreeCentrality()
{
    for(let i = 0; i < n; i++)
    {
        du.push(0);
    }
    degree.set(du);
    Tracer.delay();
    for(let i = 0; i < n; i++)
    {
        for(let j = 0; j<gp[i].length;j++)
        {
            tracer.visit(i);
            tracer.visit(gp[i][j],i);
            du[i]++;
            degree.patch(i,du[i]);
            array2dTracer.patch(i,j);
            Tracer.delay();
            degree.depatch(i,du[i]);
            du[gp[i][j]]++;
            degree.patch(gp[i][j],du[gp[i][j]]);
            Tracer.delay();
            degree.depatch(gp[i][j],du[gp[i][j]]);
            array2dTracer.depatch(i,j);
            tracer.leave(i);
            tracer.leave(gp[i][j],i);
        }
        
    }
    maxnode = 0;
    for(var i = 0; i<n; i++)
    {
        if(du[i]>du[maxnode])
        {
            maxnode = i;
        }
    }
    
    degree.select(maxnode);
    tracer.select(maxnode);
    for(let i = 0; i < n; i++)
    {
        for(let j = 0; j<gp[i].length;j++)
        {
            if(i===maxnode)
            {
                tracer.select(gp[i][j],i);
                tracer.deselect(gp[i][j]);
            }
            else if(gp[i][j]===maxnode)
            {
                tracer.select(gp[i][j],i);
            }
        }
    }
    logger.println("\nmaxdegree:"+du[maxnode]+" node:"+maxnode);
    return;
}