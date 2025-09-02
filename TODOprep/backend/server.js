// backend/server.js
const express = require("express");
const cors = require("cors");
const connectDB = require("./db");
const Todo = require("./models/Todo");

const app = express();
connectDB();

app.use(cors());
app.use(express.json());

// Get all todos
app.get("/todos", async (req, res) => {
  const todos = await Todo.find();
  res.json(todos);
});

// Create new todo
app.post("/todos", async (req, res) => {
  try {
    const { task, deadline } = req.body;
    const newTodo = new Todo({ task, deadline: deadline || null });
    await newTodo.save();
    res.status(201).json(newTodo);
  } catch (err) {
    console.error("Error creating todo:", err);
    res.status(500).json({ error: "Failed to create todo" });
  }
});


// Delete todo
app.delete("/todos/:id", async (req, res) => {
  const { id } = req.params;

  // Check for valid ObjectId before querying MongoDB
  if (!mongoose.Types.ObjectId.isValid(id)) {
    return res.status(400).json({ error: "Invalid ID format" });
  }

  try {
    const result = await Todo.findByIdAndDelete(id);

    if (!result) {
      return res.status(404).json({ error: "Todo not found" });
    }

    res.status(204).end(); // No content
  } catch (err) {
    console.error("❌ Error deleting todo:", err.message);
    res.status(500).json({ error: "Failed to delete todo" });
  }
});


const PORT = 5000;
app.listen(PORT, () => console.log(`Server running on port ${PORT}`));
