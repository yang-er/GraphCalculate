const {Tracer, Array1DTracer, Array2DTracer, GraphTracer, LogTracer, Randomize, Layout, VerticalLayout, HorizontalLayout } = require('algorithm-visualizer');

// initialize components
const graph1 = new GraphTracer().directed(true);
const MinWayNum1 = new Array2DTracer("MinWayNum");
const Shortest1 = new Array2DTracer("Shortest");
const PreNode1 = new Array2DTracer("PreNode");
const PaFb1 = new Array2DTracer("PaFb");
const Betweenness1 = new Array1DTracer("Betweenness");
const logger = new LogTracer();
const line1 = new HorizontalLayout([MinWayNum1, Shortest1]);
const line2 = new HorizontalLayout([PreNode1, PaFb1]);
const line3 = new HorizontalLayout([Betweenness1, logger]);
Layout.setRoot(new VerticalLayout([graph1, line1, line2, line3]));

// initialize graph
const N = 5;
const graph = [];
const MinWayNum = [];
const Shortest = [];
const PreNode = [];
const PaFb = [];
const vis = [];
const Betweenness = [];

for (let i = 0; i < N; i++) {
    MinWayNum[i] = [];
    Shortest[i] = [];
    PreNode[i] = [];
    PaFb[i] = [];
    vis[i] = 0;
    graph[i] = [];
    Betweenness[i] = 0;

    for (let j = 0; j < N; j++) {
        MinWayNum[i][j] = 0;
        Shortest[i][j] = Infinity;
        PreNode[i][j] = 0;
        PaFb[i][j] = 0;
        graph[i][j] = 0;
    }
}

graph[4][2] = 1;
graph[4][1] = 1;
graph[2][1] = 1;
graph[2][0] = 1;
graph[0][3] = 1;
graph[0][2] = 1;
graph[3][1] = 1;

graph1.set(graph);
MinWayNum1.set(MinWayNum);
Shortest1.set(Shortest);
PreNode1.set(PreNode);
Betweenness1.set(Betweenness);
PaFb1.set(PaFb);
Tracer.delay();

// begin
betweenness();

function betweenness() {
	var stack = new Stack();
	var queue = new Queue();

	for (let i = 0; i < N; i++) {
	    logger.println(`Finding shortest path from ${i}`);
		queue.enqueue({ u: i, dis: 0, uu: -1 });
		var visStack = new Stack();

		while (!queue.empty()) {
			let e = queue.front();
			queue.dequeue();
			//logger.println(`${e}`);
			if (Shortest[i][e.u] < e.dis) continue;
			if (e.uu != -1) {
			    graph1.visit(e.u, e.uu);
			    visStack.push(e);
			} else {
			    graph1.select(i);
		        Tracer.delay();
			}
			
			Shortest[i][e.u] = e.dis;
			Shortest1.patch(i, e.u, e.dis);
			MinWayNum[i][e.u]++;
			MinWayNum1.patch(i, e.u, MinWayNum[i][e.u]);
			Tracer.delay();

			for (let v = 0; v < N; v++) {
				for (let k = 0; k < graph[e.u][v]; k++) {
					queue.enqueue({ u: v, dis: e.dis+1, uu: e.u });
				}
			}
		}
		
		while (!visStack.empty()) {
			let e = visStack.peek();
			visStack.pop();
		    graph1.leave(e.u, e.uu);
		}
		
		graph1.deselect(i);
		
		for (let k = 0; k < N; k++) {
		    if (Shortest[i][k] != Infinity) {
		        Shortest1.depatch(i, k);
		        MinWayNum1.depatch(i, k);
		    }
		}
	}

	for (let i = 0; i < N; i++) {
	    //计算从a出发经过b的介数中心性
		queue.enqueue({ u: i, dis: 0 });
		stack.push(i);
		vis[i] = 1;
		graph1.select(i);

		while (!queue.empty())
		{
			let e = queue.front();
			queue.dequeue();

			if (Shortest[i][e.u] < e.dis)
				continue;

			for (let v = 0; v < N; v++) {
				for (let k = 0; k < graph[e.u][v]; k++) {
					queue.enqueue({ u: v, dis: e.dis+1 });
					PreNode[e.u][v] = 1;
					PreNode1.patch(e.u, v, 1);
				}

				if (graph[e.u][v] > 0 && vis[v] === 0) {
					stack.push(v);
					vis[v] = 1;
				}
			}
		}
		
		Tracer.delay();
        for (let x = 0; x < N; x++)
            for (let y = 0; y < N; y++)
                PreNode1.depatch(x, y);
        graph1.deselect(i);
        Tracer.delay();
		for (let k = 0; k < N; k++) vis[k] = 0;

		while (!stack.empty()) {
			let pre = new Queue(); //记录当前节点的前驱节点
			let flag = false; //用来标记当前节点是否为其他节点的前驱
			let u = stack.peek();
			console.log(u);
			stack.pop();

			for (let v = 0; v < N; v++) {
				if (PreNode[u][v] === 1 && v !== i) {
					flag = true;
					pre.enqueue(v);
				}
			}

			graph1.select(i);
			graph1.select(u);
			//Tracer.delay();
			var ups = [];
			
		    logger.println(`Updating PaFb from ${i} to ${u}`);
			if (flag) {
				while (!pre.empty()) {
					let j = pre.front();
					pre.dequeue();
					graph1.select(j);
					ups.push(j);
					PaFb[i][u] += (MinWayNum[i][u] / MinWayNum[i][j]) * (1 + PaFb[i][j]) * graph[u][j];
					PaFb1.patch(i, u, PaFb[i][u]);
				}
			}
			
			Tracer.delay();
			for (let jj = 0; jj < ups.length; jj++)
			    graph1.deselect(ups[jj]);
			PaFb1.depatch(i, u);
			graph1.deselect(i);
			graph1.deselect(u);
		}
	}

	for (let i = 0; i < N; i++) {
		PaFb[i][i] = 0;
		PaFb1.patch(i, i, 0);
	}
	
	Tracer.delay();
	for (let i = 0; i < N; i++)
	    PaFb1.depatch(i, i);
	Tracer.delay();
	
	for (let i = 0; i < N; i++) {
		for (let j = 0; j < N; j++) {
		    PaFb1.select(j, i);
			Betweenness[i] += PaFb[j][i];
		}
		
		Betweenness1.patch(i, Betweenness[i]);
		Tracer.delay();
		for (let j = 0; j < N; j++)
		    PaFb1.deselect(j, i);
		Betweenness1.depatch(i);
	}
	
	let max = 0;
	let node = 0;
	for (let i = 0; i < N; i++) {
		if (Betweenness[i] > max) {
			max = Betweenness[i];
			node = i;
		}
	}
	
	logger.println(`介数中心性最大的是节点 ${node}  最大的介数中心性为 ${max}`);
	Betweenness1.select(node);
	graph1.select(node);
}

function Stack() {
	return {
		dataStore: [],
		top: 0,
		pop: function() {
			return this.dataStore[--this.top];
		},
		push: function (element) {
			this.dataStore[this.top++] = element;
		},
		peek: function () {
			if( this.top > 0 ) return this.dataStore[this.top-1];
			else return 'Empty';
		},
		empty: function () {
			return this.top === 0;
		}
	};
}

function Queue() {
	return {
		dataStore: [],
		enqueue: function (element) {
			this.dataStore.push(element);
		},
		dequeue: function () {
			return this.dataStore.shift();
		},
		front: function () {
			return this.dataStore[0];
		},
		back: function () {
			return this.dataStore[this.dataStore.length-1];
		},
		empty: function () {
			return this.dataStore.length === 0;
		}
	};
}
