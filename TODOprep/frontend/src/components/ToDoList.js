// frontend/src/components/TodoList.js
import React, { useState, useEffect } from "react";
import axios from "axios";

function TodoList() {
  const [todos, setTodos] = useState([]);
  const [task, setTask] = useState("");
  const [deadline, setDeadline] = useState("");

  const fetchTodos = async () => {
    const res = await axios.get("http://localhost:5000/todos");
    setTodos(res.data);
  };

  const addTodo = async () => {
    if (!task) return;
    try {
      await axios.post("http://localhost:5000/todos", {
        task,
        deadline: deadline || null,
      });
      setTask("");
      setDeadline("");
      fetchTodos();
    } catch (err) {
      console.error("Error adding task:", err);
    }
  };

  const deleteTodo = async (id) => {
    console.log("Deleting ID:", id); // ADD THIS LINE
    try {
      await axios.delete(`http://localhost:5000/todos/${id}`);
      fetchTodos();
    } catch (err) {
      console.error("❌ Error deleting task:", err);
    }

  };


  useEffect(() => {
    fetchTodos();
  }, []);

  return (
    <div>
      <h2>To-Do List</h2>
      <input
        value={task}
        onChange={(e) => setTask(e.target.value)}
        placeholder="New Task"
      />
      <input
        type="datetime-local"
        value={deadline}
        onChange={(e) => setDeadline(e.target.value)}
        placeholder="Deadline"
      />
      <button onClick={addTodo}>Add</button>

      <ul>
        {todos.map(todo => (
          <li key={todo._id}>
            <strong>{todo.task}</strong><br />
            {todo.deadline && (
              <small>
                Deadline: {new Date(todo.deadline).toLocaleString()}
              </small>
            )}
            <br />
            <button onClick={() => deleteTodo(todo._id)}>X</button>
          </li>
        ))}
      </ul>
    </div>
  );
}

export default TodoList;
