const { Tracer, Array2DTracer, LogTracer, Layout, VerticalLayout } = require('algorithm-visualizer');

const matrix = new Array2DTracer();
const vects = new Array2DTracer();
const logger = new LogTracer();

Layout.setRoot(new VerticalLayout([matrix, vects, logger]));

const N = 5;
const eps = 4e-4;
const S = [];
const M = [];

for (let i = 0; i < N; i++) {
    S[i] = [];
    M[i] = [];
    
    for (let j = 0; j < N; j++) {
        S[i][j] = 0;
        M[i][j] = 0;
    }
    
    S[i][i] = 1;
}

M[0][1] = M[1][0] = M[0][2] = M[2][0] = 1;
M[0][4] = M[4][0] = 2;
M[1][2] = M[2][1] = M[2][3] = M[3][2] = 1;
M[2][4] = M[4][2] = 1;

vects.set(S);
matrix.set(M);
Tracer.delay();

///////INITIALIZED///////////

function setMatrix(i, j, v) {
	M[i][j] = v;
	matrix.patch(i, j, v);
}

function setVects(i, j, v) {
	S[i][j] = v;
	vects.patch(i, j, v);
}

function Jacobi() {
	let nCount = 0;
	
	while (true) {
	    let dbMax = M[0][1];
	    let nRow = 0;
	    let nCol = 1;
		for (let i = 0; i < N; i++) {
			for (let j = 0; j < N; j++) {
				let d = Math.abs(M[i][j]);
				if ((i !== j) && (d > dbMax)) {
					dbMax = d;
					nRow = i;
					nCol = j;
				}
			}
		}
		
		if (dbMax < eps) {
			logger.println(`EPS achieved.`);
			break;
		}
		
		nCount++;
		logger.println(`Round ${nCount}: abs(M[${nRow}][${nCol}]) = ${dbMax}`);
		
		matrix.select(nRow, nCol);
		Tracer.delay();
		
		let dbApp = M[nRow][nRow];
		let dbApq = M[nRow][nCol];
		let dbAqq = M[nCol][nCol];
		
		let dbAngle = 0.5 * Math.atan2(-2 * dbApq, dbAqq - dbApp);
		let dbSinTheta = Math.sin(dbAngle);
		let dbCosTheta = Math.cos(dbAngle);
		let dbSin2Theta = Math.sin(2 * dbAngle);
		let dbCos2Theta = Math.cos(2 * dbAngle);
		
		setMatrix(nRow, nRow, dbApp * dbCosTheta * dbCosTheta + dbAqq * dbSinTheta * dbSinTheta + 2 * dbApq * dbCosTheta * dbSinTheta);
		setMatrix(nCol, nCol, dbApp * dbSinTheta * dbSinTheta + dbAqq * dbCosTheta * dbCosTheta - 2 * dbApq * dbCosTheta * dbSinTheta);
		setMatrix(nRow, nCol, 0.5 * (dbAqq - dbApp) * dbSin2Theta + dbApq * dbCos2Theta);
		setMatrix(nCol, nRow, 0.5 * (dbAqq - dbApp) * dbSin2Theta + dbApq * dbCos2Theta);
		
		for (let i = 0; i < N; i++) {
			if ((i != nCol) && (i != nRow)) {
				let db = M[i][nRow];
				setMatrix(i, nRow, M[i][nCol] * dbSinTheta + db * dbCosTheta);
				setMatrix(i, nCol, M[i][nCol] * dbCosTheta - db * dbSinTheta);
			}
		}
		
		for (let j = 0; j < N; j++) {
			if ((j != nCol) && (j != nRow)) {
				let db = M[nRow][j];
				setMatrix(nRow, j, M[nCol][j] * dbSinTheta + db * dbCosTheta);
				setMatrix(nCol, j, M[nCol][j] * dbCosTheta - db * dbSinTheta);
			}
		}
		
		for (let i = 0; i < N; i++) {
			let db = S[i][nRow];
			setVects(i, nRow, S[i][nCol] * dbSinTheta + db * dbCosTheta);
			setVects(i, nCol, S[i][nCol] * dbCosTheta - db * dbSinTheta);
		}
		
		Tracer.delay();
		for (let i = 0; i < N; i++) {
			matrix.depatch(i, nCol);
			matrix.depatch(nCol, i);
			matrix.depatch(i, nRow);
			matrix.depatch(nRow, i);
			vects.depatch(i, nCol);
			vects.depatch(nCol, i);
			vects.depatch(i, nRow);
			vects.depatch(nRow, i);
		}
		
		matrix.deselect(nRow, nCol);
	}
	
	for (let i = 0; i < N; i++) {
		for (let j = 0; j < N; j++) {
			if (Math.abs(M[i][j]) < eps) {
				setMatrix(i, j, 0);
				matrix.depatch(i, j);
			}
			if (Math.abs(S[i][j]) < eps) {
				setVects(i, j, 0);
				vects.depatch(i, j);
			}
		}
	}
}

Jacobi();
Tracer.delay();
let maxV = 0;

for (let i = 1; i < N; i++) {
	if (M[i][i] > M[maxV][maxV]) {
		maxV = i;
	}
}

matrix.select(maxV, maxV);

for (let i = 0; i < N; i++) {
	vects.select(i, maxV);
}

logger.println(`max eigen value = ${M[maxV][maxV]}`)
