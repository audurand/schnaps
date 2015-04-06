# SCHNAPS
SynCHroNous Agent- and Population-based Simulator

Open source simulator:
  * multiagent
  * based on populations
  * time-driven
  * extensible
  * command line and XML configurable.

## Plugins

### Operators
Mathematical, logical, and relational operators as primitives for function tree description.

### Control
Control flow operators such as switch cases, conditions and process call/push.

### Data
Reference (get) and explicit modificators (set) to parameters, variables and other simulation data.

### Meds
Medical primitives used for these simulations:
  * osteoporosis
  * Down's syndrome
  * diabetic retinopathy
  * random screening.

### Learning
This plugin enables using the simulator for Monte Carlo reinforcement learning.

## Applications
Several medical decision applications have been used to validate the simulator and its plugins.

The simulation system, the basic plugins (`Operators`, `Control` and `Data`) and the medical plugin `Meds` have been validated using several medical decision applications. The simulation models have been designed in collaboration with health care specialists. The output results have also been validated by health care professionals based on the literature. The simulation parameters describing these applications can be found [here](http://code.google.com/p/schnaps/source/browse/#hg%2FApplications%2FSCHNAPS).

The `Learning` plugin has been validated using real medical applications and synthetic application. The simulation parameters describing these applications can be found [here](http://code.google.com/p/schnaps/source/browse/#hg%2FApplications%2FSCHNAPS-Learning).

## How to cite

Authors of scientific papers based on results using SCHNAPS are encouraged to cite the following paper.

<pre>
@article{Durand2012,
  title={Population-Based Simulation for Public Health: Generic Software Infrastructure and Its Application to Osteoporosis},
  author={Durand, Audrey and Gagn\'e, Christian and Nshimyumukiza, L\'eon and Gagnon, Mathieu and Rousseau, Fran\c{c}ois and Gigu\`ere, Yves and Reinharz, Daniel},
  journal={IEEE Transactions on Systems, Man and Cybernetics, Part A: Systems and Humans},
  volume={42},
  number={6},
  pages={1396--1409},
  year={2012}
}
</pre>

## Publications on SCHNAPS

  * Audrey Durand, Christian Gagné, Léon Nshimyumukiza, Mathieu Gagnon, François Rousseau, Yves Giguère and Daniel Reinharz, "Population-based simulation for public health: Generic software infrastructure and its application to osteoporosis", _IEEE Transactions on Systems, Man and Cybernetics_, Part A: Systems and Humans, vol. 42, no. 6, pp. 1396-1409, 2012.
  * Audrey Durand, Christian Gagné, Marc-André Gardner, François Rousseau, Yves Giguère, and Daniel Reinharz, "SCHNAPS: a generic population-based simulator for public health purposes," in _Proceedings of Summer Computer Simulation Conference_, 2010, pp. 182–187.

## Projects using SCHNAPS

  * Léon Nshimyumukiza, Antoine Bois, Patrick Daigneault, Larry C. Lands, Anne-Marie Laberge, Diane Fournier, Julie Duplantie, Yves Giguère, Jean Gekas, Christian Gagné, François Rousseau, Daniel Reinharz, "Cost effectiveness of newborn screening for cystic fibrosis: A simulation study", _Journal of Cystic Fibrosis_, In Press, 8 pages, 2013.

  * Julie Duplantie, Odilon Martinez Gonzales, Antoine Bois, Léon Nshimyumukiza, Jean Gekas, Emmanuel Bujold, Valérie Morin, Maude Vallée, Yves Giguère, and Christian Gagné, "Cost-effectiveness of the management of Rh-negative pregnant women", _Journal of Obstetrics and Gynaecology Canada (JOGC)_, vol. 35, no. 8, pp. 730-740, 2013.

  * Léon Nshimyumukiza, Julie Duplantie, Mathieu Gagnon, Xavier Douville, Diane Fournier, Carmen Lindsay, Marc Parent, Alain Milot, Yves Giguère, Christian Gagné, François Rousseau, Daniel Reinharz, "Dabigatran versus warfarin under standard or pharmacogenetic-guided management for the prevention of stroke and systemic thromboembolism in patients with atrial fibrillation: a cost/utility analysis using an analytic decision model", _Thrombosis journal_, vol. 11, 14 pages, 2013.

  * Léon Nshimyumukiza, Audrey Durand, Mathieu Gagnon, Xavier Douville, Suzanne Morin, Carmen Lindsay, Julie Duplantie, Christian Gagné, Sonia Jean, Yves Giguère, Sylvie Dodin, François Rousseau and Daniel Reinharz, "An economic evaluation: Simulation of the cost/effectiveness and cost/utility of universal prevention strategies against osteoporosis-related fractures", _Journal of Bone and Mineral Research_, vol. 28, no. 2, pp. 383-394, 2013.
