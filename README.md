# Perceptron based Branch Predictor
In modern architectures, the processors operate at high clock speeds with deep pipelines. Since branch mispredictions result in flushing of the pipeline, performance deteriorates significantly if the misprediction rate is high. Hence, accurate branch prediction is critical to improving performance using speculative execution.

The branch predictor implementation is based on the ideas described in this [paper](https://www.cs.utexas.edu/~lin/papers/hpca01.pdf).
My implementation achieved a mpki (mispredictions per thousand instructions) of around 4.5 and stood among the top 5 in a class of 60
graduate students.
