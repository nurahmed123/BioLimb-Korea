import mongoose from 'mongoose';

const patientSchema = new mongoose.Schema({
  fullName: {
    type: String,
    required: [true, 'Please provide full name'],
  },
  email: {
    type: String,
    required: [true, 'Please provide email'],
    match: [/^\S+@\S+\.\S+$/, 'Please provide a valid email'],
  },
  phoneNumber: {
    type: String,
    required: [true, 'Please provide phone number'],
  },
  address: {
    type: String,
    required: [true, 'Please provide address'],
  },
  photo: {
    type: String,
    default: null,
  },
  pulseRate: {
    type: Number,
    default: null,
  },
  oxygenLevel: {
    type: Number,
    default: null,
  },
  bodyTemperature: {
    type: Number,
    default: null,
  },
  sampleTaken: {
    type: Boolean,
    default: false,
  },
  testReport: {
    type: String,
    enum: ['Positive', 'Negative'],
    default: null,
  },
  createdAt: {
    type: Date,
    default: Date.now,
  },
  updatedAt: {
    type: Date,
    default: Date.now,
  },
});

// Update the updatedAt timestamp before saving
patientSchema.pre('save', function(next) {
  this.updatedAt = new Date();
  next();
});

export default mongoose.models.Patient || mongoose.model('Patient', patientSchema); 