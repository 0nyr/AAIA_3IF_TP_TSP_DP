using CairoMakie
using GraphMakie
using Graphs
using LaTeXStrings

fig = Figure()

NB_NODES = 4

# Create a simple complete graph
g = SimpleGraph(NB_NODES)
for i in 1:NB_NODES
    for j in i+1:NB_NODES
        add_edge!(g, i, j)
    end
end

# Define LaTeX-compatible labels for nodes
node_labels = []
for i in 1:NB_NODES
    node_name = i - 1
    push!(node_labels, L"%$node_name")
end

# Specify node positions
positions = [
    Point2f(0, 1),   # Node 1
    Point2f(1, 1),   # Node 2
    Point2f(0, 0),   # Node 3
    Point2f(1, 0),   # Node 4
]

# Plot the graph
fig[1, 1] = Label(
        fig, 
        text = "Customer-based complete graph.", 
        fontsize = 24
)

ax =  Axis(
    fig[2, 1],
    limits = (-0.2, 1.2, -0.2, 1.2)
)
graphplot!(
    ax,
    g,
    layout = positions,
    ilabels = node_labels,
    node_size = 40,
    node_label_size = 18,
    edge_label_size = 14
)

hidedecorations!(ax)
hidespines!(ax)

# Save figure as SVG on disk
save("img/4_vertices_simple_complete_customer_based_graph.png", fig)