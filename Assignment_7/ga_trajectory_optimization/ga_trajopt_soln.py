import numpy as np
from ga_trajopt import GATrajectoryOptimizer


class GATrajectoryOptimizerSolution(GATrajectoryOptimizer):

    def fitness(self, x):
        fit = 0
        # --- Your code here
        X = self.start[np.newaxis, :] + np.cumsum(x, axis = 0)
        xT = X[-1] 
        xt_norm_sum = np.sum((np.linalg.norm(x))**2, axis=0)
        sum_obstacle = 0
        for i in range(x.shape[0]):
            dist1 = np.linalg.norm(X[i] - self.obstacle_centres[0])
            dist2 = np.linalg.norm(X[i] - self.obstacle_centres[1])
            obstacle_x = (dist1 <= self.obstacle_radii[0]) | (dist2 <= self.obstacle_radii[1])
            obstacle_x = obstacle_x.astype(int)
            sum_obstacle += obstacle_x
        fit = -((np.square(np.linalg.norm(xT - self.goal))) + xt_norm_sum + 100*(sum_obstacle))

        # ---
        return fit

    def crossover(self, parent1, parent2, crossover_point):
        child1, child2 = None, None
        # --- Your code here
        child1 = np.concatenate((parent1[:crossover_point],parent2[crossover_point:]))
        child2 = np.concatenate((parent2[:crossover_point],parent1[crossover_point:]))
        # ---
        return child1, child2

    def select_children(self, parent, child):

        chosen, fitness = None, None

        # --- Your code here
        fit_child = self.fitness(child)
        fit_parent = self.fitness(parent) 
        if np.random.rand() < self.params['select_best_child_p']:
            if fit_child < fit_parent:
                chosen, fitness = parent, fit_child
            else:
                chosen, fitness = child, fit_child
        
        else:
            chosen, fitness = parent, fit_parent

        # ---
        return chosen, fitness


if __name__ == "__main__":
    # GA path planning

    # Setup start, goal and spherical obstacles
    goal = np.array([1.0, 1.0])
    start = np.array([0.0, 0.0])
    obstacle_centres = np.array([
        [0.5, 0.2],
        [0.2, 0.8]
    ])
    obstacle_radii = np.array([0.25, 0.15])

    # Solver parameters
    N = 500
    T = 20
    params = {
        'N': 500,
        'iters': 100,
        'cross_over_p': 0.5,
        'mutate_p': 0.5,
        'select_best_parent_p': 0.9,
        'select_best_child_p': 0.9
    }

    solver = GATrajectoryOptimizerSolution(start, goal, obstacle_radii, obstacle_centres, T, params)
    # Solve to get best trajectory and plot
    best = solver.solve()
    solver.plot_trajectory(best)
