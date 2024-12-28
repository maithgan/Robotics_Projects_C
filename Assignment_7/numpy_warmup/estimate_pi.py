import numpy as np


def estimate_pi(N):
    pi_estimate = None
    # --- Your code here 
    x_points = np.random.uniform(0, 1, N)
    y_points = np.random.uniform(0, 1, N)
    points_inside_circle = ((x_points - 0.5)**2 + (y_points - 0.5)**2) <= 0.5**2
    count_inside_circle = np.sum(points_inside_circle)  
    pi_estimate = 4 * count_inside_circle / N
    # ---

    return pi_estimate


if __name__ == "__main__":
    N = 1000
    approx_pi = estimate_pi(N)
    print(f'Estimate of pi is {approx_pi}')