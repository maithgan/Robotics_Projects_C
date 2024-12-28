import numpy as np
import unittest
from ga_trajopt_soln import GATrajectoryOptimizerSolution

# GA path planning
goal = np.array([1.0, 1.0])
start = np.array([0.0, 0.0])
T = 20
obstacle_centres = np.array([
    [0.5, 0.2],
    [0.2, 0.8]
])

obstacle_radii = np.array([0.25, 0.15])

params = {
    'N': 500,
    'iters': 500,
    'cross_over_p': 0.1,
    'mutate_p': 0.9,
    'select_best_parent_p': 0.9,
    'select_best_child_p': 1
}

class GATestCase(unittest.TestCase):
    solver = GATrajectoryOptimizerSolution(start, goal, obstacle_radii, obstacle_centres, T, params)

    def test_crossover(self):
        x1 = np.array([[-0.75521622,  1.96943781],
                        [ 1.21398347, -1.49826399],
                        [-1.64279651,  1.07287028],
                        [-0.15861844, -0.12024467],
                        [ 1.13028404,  0.83371871],
                        [-0.17847927,  0.95494971],
                        [-0.26509163,  0.31920206],
                        [-0.11069506,  1.74775521],
                        [-1.94005007,  1.39515542],
                        [-0.86522317,  0.93313183]])
        x2 = np.array([[ 0.96958918, -0.67243311],
                        [-0.32380719,  0.81874302],
                        [ 1.41265162,  0.50383653],
                        [-2.46772521,  1.00482059],
                        [ 0.0195284 , -0.1221559 ],
                        [-0.75073382,  1.48664963],
                        [-0.34729427, -0.27731726],
                        [ 0.57418684,  0.25226723],
                        [ 1.08294403, -0.57059223],
                        [ 0.42949525, -0.50935447]])

        y1_correct = np.array([[-0.75521622,  1.96943781],
                                [ 1.21398347, -1.49826399],
                                [-1.64279651,  1.07287028],
                                [-0.15861844, -0.12024467],
                                [ 1.13028404,  0.83371871],
                                [-0.17847927,  0.95494971],
                                [-0.26509163,  0.31920206],
                                [ 0.57418684,  0.25226723],
                                [ 1.08294403, -0.57059223],
                                [ 0.42949525, -0.50935447]])

 
        y2_correct = np.array([[ 0.96958918, -0.67243311],
                                [-0.32380719,  0.81874302],
                                [ 1.41265162,  0.50383653],
                                [-2.46772521,  1.00482059],
                                [ 0.0195284, -0.1221559 ],
                                [-0.75073382,  1.48664963],
                                [-0.34729427, -0.27731726],
                                [-0.11069506,  1.74775521],
                                [-1.94005007,  1.39515542],
                                [-0.86522317,  0.93313183]])

        y1, y2 = self.solver.crossover(x1, x2, 7)
 
        self.assertTrue(np.allclose(y1, y1_correct))
        self.assertTrue(np.allclose(y2, y2_correct))

    def test_fitness(self):
        x = np.array([
            [-1.04372735, -0.2090537 ],
            [-0.02928477,  1.05727152],
            [-0.65099578,  0.41695897],
            [ 2.49625825, -0.75230825]])
        # rounded to 2 decimal places
        fitness = round(self.solver.fitness(x) * 100) / 100.0
        self.assertAlmostEqual(fitness, -9.94)

    def test_select_chidren(self):
        parent = np.array([[ 0.10407687, -0.10668349],
                        [-0.17903166, -0.03602064],
                        [-0.02689409,  0.10092332],
                        [-0.05855341,  0.22569848],
                        [-0.05382966, -0.15515566]])

        child = np.array([[-0.04523977,  0.17915973],
                        [ 0.02400511, -0.00303814],
                        [ 0.00249622,  0.02107638],
                        [ 0.08224441, -0.01710187],
                        [ 0.02362649,  0.0092048 ]])
        fitness_child = -1.53
        fitness_parent = -2.57

        # always choose best
        self.solver.params['select_best_child_p'] = 1
        chosen, fitness = self.solver.select_children(parent, child)

        self.assertTrue(np.allclose(chosen, child))
        self.assertAlmostEqual(round(fitness*100) / 100, fitness_child)
        
        # always choose worst
        self.solver.params['select_best_child_p'] = 0
        chosen, fitness = self.solver.select_children(parent, child)
   
        self.assertTrue(np.allclose(chosen, parent))
        self.assertAlmostEqual(round(fitness*100) / 100, fitness_parent)

if __name__ == "__main__":
    unittest.main()
    