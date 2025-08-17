import { NextResponse } from 'next/server';
import connectDB from '@/lib/mongodb';
import Patient from '@/models/Patient';

// GET all patients
export async function GET() {
  try {
    await connectDB();
    const patients = await Patient.find({});
    
    return NextResponse.json({
      success: true,
      patients
    });
  } catch (error) {
    console.error('Error fetching patients:', error);
    return NextResponse.json({
      success: false,
      message: 'Failed to fetch patients',
      error: error instanceof Error ? error.message : 'Unknown error'
    }, { status: 500 });
  }
}

// POST new patient
export async function POST(request: Request) {
  try {
    const formData = await request.json();
    const { fullName, email, phoneNumber, address, photo } = formData;

    await connectDB();

    // Create patient document
    const patient = await Patient.create({
      fullName,
      email,
      phoneNumber,
      address,
      photo: photo ? photo : null,
    });

    return NextResponse.json({
      success: true,
      message: 'Patient registered successfully',
      patientId: patient._id
    }, { status: 201 });

  } catch (error) {
    console.error('Error registering patient:', error);
    return NextResponse.json({
      success: false,
      message: 'Failed to register patient',
      error: error instanceof Error ? error.message : 'Unknown error'
    }, { status: 500 });
  }
} 