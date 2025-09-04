// backend/models/Todo.js
const mongoose = require("mongoose");

const TodoSchema = new mongoose.Schema({
  task: {
    type: String,
    required: true,
  },
  deadline: {
    type: Date, // store both date & time
    required: false,
  }
});

module.exports = mongoose.model("Todo", TodoSchema);
