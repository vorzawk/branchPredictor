/* Abhilash Vallamkonda
   126004453	*/

// my_predictor.h
// This file contains the my_predictor class for the perceptron branch predictor.
// history length is 128.

class my_update : public branch_update {
public:
	unsigned int index;
};
class my_predictor : public branch_predictor {
public:
#define HISTORY_LENGTH	64
#define THRESHOLD (2 * HISTORY_LENGTH + 14)
#define NR_PERCEPTRONS 1024

	my_update u;
	branch_info bi;
	unsigned int i, j;
	int y;
	short int weights [NR_PERCEPTRONS][HISTORY_LENGTH + 1];
	char history [HISTORY_LENGTH];
	// initialize the global branch history as taken
	my_predictor (void) {
		memset (history, 1, HISTORY_LENGTH); 
	}
	// The taken outcome is represented as 1 while the not taken outcome is -1
	branch_update *predict (branch_info & b) {
		bi = b;
		/* check for conditional branches and make a prediction based 
		 on the global history and the learnt weights */
		if (b.br_flags & BR_CONDITIONAL) {
			/* use the LSBs of the branch address to index into 
			the table of weights */
			u.index = b.address & (NR_PERCEPTRONS-1);
			// The first weight measures the bias of the current branch.
			y = weights[u.index][0];
			/* The remaining weights are used along with the global 
			history to update the perceptron output which is later 
			used to make the prediction */
			for (j = 1; j <= HISTORY_LENGTH; j++)	{
				y += weights[u.index][j] * history[j-1];
			}
			// Predict taken if the perceptron output is greater than 0
			u.direction_prediction (y >= 0);
		} else {
			u.direction_prediction (true);
		}
		u.target_prediction (0);
		return &u;
	}

	void update (branch_update *u, bool taken, unsigned int target) {
		unsigned int index = bi.address & (NR_PERCEPTRONS - 1);
		// update the weights and history based on the outcome of the branch
		if (bi.br_flags & BR_CONDITIONAL) {
			/* update the weights in case y is less than the 
			threshold or if there is a misprediction */
			if ((abs(y) < THRESHOLD) || (taken != (y >= 0)))	{
			/* increase the bias towards taken if the branch is taken or 
			reduce it if it is not taken */
				if (taken)	{
					weights[index][0]++;
				} else {
					weights[index][0]--;
				}
			/* The weights represent the correlations between the outcome of 
			the current branch and the previous branches. The weights are 
			increased if the outcome of the current branch follows the 
			previous brasanch outcomes and decreased if not.*/
				for (j=1; j <= HISTORY_LENGTH; j++)	{
					if (taken? (history[j-1] == 1) : (history[j-1] == -1))	{
						weights[index][j] ++;
					} else {
						weights[index][j] --;
					}
			/* In steady state, the weights have a large magnitude if there 
			exists a strong correllation between the branches or a small 
			value close to 0 if not */
				}
			}
			
			// update the global history vector with the current outcome
			for (i = 0; i < HISTORY_LENGTH - 1; i++)	{
				history [i] = history [i+1];
			}
			if (taken) {
				history [HISTORY_LENGTH - 1] = 1;
			} else {
				history [HISTORY_LENGTH - 1] = -1;
			}
		}
	}
};
