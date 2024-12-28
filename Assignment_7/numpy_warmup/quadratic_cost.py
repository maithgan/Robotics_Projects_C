import os
import time
import numpy as np


def quadratic_cost(Q, x):
    T, d = x.shape
    cost_sum = 0
    for t in range(T):
        for i in range(d):
            for j in range(d):
                cost_sum += x[t][i] * Q[i][j] * x[t][j]
    return cost_sum


def quadratic_cost_vectorized(Q, x):
    cost_sum = 0
    # --- Your code here
    cost_sum = np.sum(np.sum(x @ Q * x, axis=1)) 
    # ---
    return cost_sum


def time_function_call(func, args):
    start = time.time()
    func(*args)
    end = time.time()
    return end - start


if __name__ == "__main__":
    # Change the file here to switch between different examples
    data = np.load(os.path.join(os.path.dirname(__file__), 'example2.npz'))

    x, Q, cost = data['x'], data['Q'], data['cost']

    # non vectorized version
    cost_nonvectorized = quadratic_cost(Q, x)
    compute_time_nonvectorized = time_function_call(quadratic_cost, (Q, x))

    # vectorized version
    cost_vectorized = quadratic_cost_vectorized(Q, x)
    compute_time_vectorized = time_function_call(quadratic_cost_vectorized, (Q, x))

    cost_value_correct = np.isclose(cost_vectorized, cost_nonvectorized)
    print(f'Non vectorized quadratic cost: {cost_nonvectorized}  Vectorized quadratic cost {cost_vectorized}')
    print(f'Speed up of vectorized vs non vectorized: {compute_time_nonvectorized / compute_time_vectorized:.2f}X')