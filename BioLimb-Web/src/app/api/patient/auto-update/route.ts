import { NextResponse } from 'next/server';
import connectDB from '@/lib/mongodb';
import Patient from '@/models/Patient';

// Function to generate random number within a range
function getRandomNumber(min: number, max: number): number {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

export async function POST(request: Request) {
  try {
    const { patientId } = await request.json();

    if (!patientId) {
      return NextResponse.json({
        success: false,
        message: 'Patient ID is required'
      }, { status: 400 });
    }

    await connectDB();

    // Generate random but realistic values
    const pulseRate = getRandomNumber(60, 100); // Normal resting heart rate range
    const oxygenLevel = getRandomNumber(95, 100); // Normal oxygen saturation range

    // Update patient document with automatic values
    const patient = await Patient.findByIdAndUpdate(
      patientId,
      {
        pulseRate,
        oxygenLevel,
        bodyTemperature: null,
        sampleTaken: true,
        updatedAt: new Date()
      },
      { new: true }
    );

    if (!patient) {
      return NextResponse.json({
        success: false,
        message: 'Patient not found'
      }, { status: 404 });
    }

    return NextResponse.json({
      success: true,
      message: 'Patient data updated automatically',
      data: {
        pulseRate,
        oxygenLevel,
        bodyTemperature: null,
        sampleTaken: true
      }
    });

  } catch (error) {
    console.error('Error in auto-update:', error);
    return NextResponse.json({
      success: false,
      message: 'Failed to update patient data',
      error: error instanceof Error ? error.message : 'Unknown error'
    }, { status: 500 });
  }
} 