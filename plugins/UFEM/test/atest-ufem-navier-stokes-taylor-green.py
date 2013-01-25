import sys
import coolfluid as cf

# Some shortcuts
root = cf.Core.root()
env = cf.Core.environment()

# Global confifuration
env.assertion_throws = False
env.assertion_backtrace = False
env.exception_backtrace = False
env.regist_signal_handlers = False
env.log_level = 4

# setup a model
model = root.create_component('NavierStokes', 'cf3.solver.ModelUnsteady')
domain = model.create_domain()
physics = model.create_physics('cf3.UFEM.NavierStokesPhysics')
solver = model.create_solver('cf3.UFEM.Solver')

# Add the Navier-Stokes solver as an unsteady solver
ns_solver = solver.add_unsteady_solver('cf3.UFEM.NavierStokes')

segments = 40

# Generate mesh
blocks = domain.create_component('blocks', 'cf3.mesh.BlockMesh.BlockArrays')
points = blocks.create_points(dimensions = 2, nb_points = 6)
points[0]  = [-0.5, -0.5]
points[1]  = [0.5, -0.5]
points[2]  = [-0.5, 0.]
points[3]  = [0.5, 0.]
points[4]  = [-0.5,0.5]
points[5]  = [0.5, 0.5]

block_nodes = blocks.create_blocks(2)
block_nodes[0] = [0, 1, 3, 2]
block_nodes[1] = [2, 3, 5, 4]

block_subdivs = blocks.create_block_subdivisions()
block_subdivs[0] = [segments, segments/2]
block_subdivs[1] = [segments, segments/2]

gradings = blocks.create_block_gradings()
gradings[0] = [1., 1., 1., 1.]
gradings[1] = [1., 1., 1., 1.]

left_patch = blocks.create_patch_nb_faces(name = 'left', nb_faces = 2)
left_patch[0] = [2, 0]
left_patch[1] = [4, 2]

bottom_patch = blocks.create_patch_nb_faces(name = 'bottom', nb_faces = 1)
bottom_patch[0] = [0, 1]

top_patch = blocks.create_patch_nb_faces(name = 'top', nb_faces = 1)
top_patch[0] = [5, 4]

right_patch = blocks.create_patch_nb_faces(name = 'right', nb_faces = 2)
right_patch[0] = [1, 3]
right_patch[1] = [3, 5]

blocks.partition_blocks(nb_partitions = cf.Core.nb_procs(), direction = 0)

mesh = domain.create_component('Mesh', 'cf3.mesh.Mesh')
blocks.create_mesh(mesh.uri())

partitioner = domain.create_component('Partitioner', 'cf3.mesh.actions.PeriodicMeshPartitioner')
partitioner.mesh = mesh

link_horizontal = partitioner.create_link_periodic_nodes()
link_horizontal.source_region = mesh.topology.right
link_horizontal.destination_region = mesh.topology.left
link_horizontal.translation_vector = [-1., 0.]

link_vertical = partitioner.create_link_periodic_nodes()
link_vertical.source_region = mesh.topology.top
link_vertical.destination_region = mesh.topology.bottom
link_vertical.translation_vector = [0., -1.]

partitioner.execute()

ns_solver.regions = [mesh.topology.interior.uri()]

Ua = 0.
Va = 0.
D = 0.5

solver.create_fields()

#initial condition for the velocity. Unset variables (i.e. the pressure) default to zero
ic_u = solver.InitialConditions.create_initial_condition(builder_name = 'cf3.UFEM.InitialConditionFunction', field_tag = 'navier_stokes_solution')
ic_u.variable_name = 'Velocity'
ic_u.regions = [mesh.topology.interior.uri()]
ic_u.value = ['{Ua} - cos(pi/{D}*x)*sin(pi/{D}*y)'.format(Ua = Ua, D = D), '{Va} + sin(pi/{D}*x)*cos(pi/{D}*y)'.format(Va = Va, D = D)]

ic_p = solver.InitialConditions.create_initial_condition(builder_name = 'cf3.UFEM.InitialConditionFunction', field_tag = 'navier_stokes_solution')
ic_p.regions = [mesh.topology.interior.uri()]
ic_p.variable_name = 'Pressure'
ic_p.value = ['-0.25*(cos(2*pi/{D}*x) + cos(2*pi/{D}*y))'.format(D = D)]

ic_u.execute()
ic_p.execute()
domain.write_mesh(cf.URI('taylor-green-init.pvtu'))

# Physical constants
physics.options().set('density', 1.)
physics.options().set('dynamic_viscosity', 0.001)
physics.options().set('reference_velocity', 1.)

# Time setup
tstep = 0.1
time = model.create_time()
time.options().set('time_step', tstep)

#ns_solver.options().set('disabled_actions', ['SolveLSS'])

# Setup a time series write
final_end_time = 10.*tstep
save_interval = tstep
current_end_time = 0.
iteration = 0

while current_end_time < final_end_time:
  current_end_time += save_interval
  time.options().set('end_time', current_end_time)
  model.simulate()
  domain.write_mesh(cf.URI('taylor-green-' +str(iteration) + '.pvtu'))
  iteration += 1
  if iteration == 1:
    solver.options().set('disabled_actions', ['InitialConditions'])

# print timings
model.print_timing_tree()
