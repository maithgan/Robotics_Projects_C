import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Circle


class GATrajectoryOptimizer:

    def __init__(self, start, goal, obstacle_radii, obstacle_centres, T, params):
        self.start = start
        self.goal = goal
        self.obstacle_radii = obstacle_radii
        self.obstacle_centres = obstacle_centres
        self.T = T
        self.population = 0.1 * np.random.randn(params['N'], T, 2)
        self.params = params

    def mutate(self, x):
        return x + 0.025 * np.random.randn(*x.shape)

    def tournament_selection(self, population, fitness):
        parents = []
        # sample idx
        for _ in range(self.params['N']):
            idx = np.random.permutation(self.params['N'])[:2]
            sorted_idx = np.argsort(fitness[idx])
            if np.random.rand() < self.params['select_best_parent_p']:
                winner_idx = idx[sorted_idx[-1]]
            else:
                winner_idx = idx[sorted_idx[-2]]
            parents.append(population[winner_idx])

        return np.stack(parents, axis=0)

    def generate_children(self, parent1, parent2):
        r_xover = np.random.rand()
        r_mutate = np.random.rand()
        xover_point = np.random.randint(1, self.T - 1)
        if r_xover < self.params['cross_over_p']:
            child1, child2 = self.crossover(parent1, parent2, xover_point)
        else:
            child1, child2 = parent1, parent2

        if r_mutate < self.params['mutate_p']:
            child1, child2 = self.mutate(child1), self.mutate(child2)

        return child1, child2
   
    def solve(self):
        # compute fitness of initial population
        fitness = np.zeros(self.params['N'])
        for n in range(self.params['N']):
            fitness[n] = self.fitness(self.population[n])

        for i in range(self.params['iters']):

            # first select parents
            parents = self.tournament_selection(self.population, fitness)

            # split parents into two groups
            parent1, parent2 = parents[:self.params['N'] // 2], parents[self.params['N'] // 2:]

            for n, (p1, p2) in enumerate(zip(parent1, parent2)):
                child1, child2 = self.generate_children(p1, p2)

                child1, child1_fitness = self.select_children(child1, p1)
                child2, child2_fitness = self.select_children(child2, p2)

                self.population[2 * n] = child1
                self.population[2 * n + 1] = child2
                fitness[2 * n] = child1_fitness
                fitness[2 * n + 1] = child2_fitness

            print(f'Iteration: {i},  Av fitness: {np.mean(fitness):.3f}  Best fitness: {np.max(fitness):.3f}')

        best_idx = np.argmax(fitness)
        return self.population[best_idx]

    def plot_trajectory(self, x):
        _x = np.concatenate((self.start.reshape(1, 2), x), axis=0)
        _x = np.cumsum(_x, axis=0)
        fig, ax = plt.subplots()
        for c, r in zip(self.obstacle_centres, self.obstacle_radii):
            circ = plt.Circle(c, r, color='k')
            ax.add_patch(circ)

        plt.scatter(self.start[0], self.start[1], s=200, marker='*', color='g')
        plt.scatter(self.goal[0], self.goal[1], s=200, marker='*', color='b')
        plt.xlim([-0.4, 1.2])
        plt.ylim([-0.4, 1.2])

        plt.plot(_x[:, 0], _x[:, 1])
        plt.show()

