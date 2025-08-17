import { NextResponse } from 'next/server';
import connectDB from '@/lib/mongodb';
import Patient from '@/models/Patient';
import { sendTestResultEmail } from '@/lib/email-service';

// PUT (update) patient
export async function PUT(
  request: Request,
  { params }: { params: { id: string } }
) {
  try {
    // Await the params object
    const { id } = await Promise.resolve(params);
    const updates = await request.json();

    // Validate required fields
    const requiredFields = ['fullName', 'email', 'phoneNumber', 'address'];
    for (const field of requiredFields) {
      if (!updates[field]) {
        return NextResponse.json({
          success: false,
          message: `${field} is required`
        }, { status: 400 });
      }
    }

    await connectDB();

    // Get the current patient data to check if test result is being updated
    const currentPatient = await Patient.findById(id);
    
    if (!currentPatient) {
      return NextResponse.json({
        success: false,
        message: 'Patient not found'
      }, { status: 404 });
    }

    // Remove _id from updates to prevent modifying immutable field
    const { _id, ...updateData } = updates;

    // Update patient document
    const patient = await Patient.findByIdAndUpdate(
      id,
      {
        ...updateData,
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

    // If test result is being updated and it's different from the current one
    if (updates.testReport && currentPatient.testReport !== updates.testReport) {
      console.log('Test result updated, attempting to send email...');
      console.log('Email details:', {
        to: updates.email,
        name: updates.fullName,
        result: updates.testReport
      });

      try {
        // Send email notification
        const emailSent = await sendTestResultEmail(
          updates.email,
          updates.fullName,
          updates.testReport as 'Positive' | 'Negative'
        );

        if (!emailSent) {
          console.error('Failed to send test result email to:', updates.email);
        } else {
          console.log('Email sent successfully to:', updates.email);
        }
      } catch (emailError) {
        console.error('Error sending email:', emailError);
      }
    }

    return NextResponse.json({
      success: true,
      message: 'Patient updated successfully',
      patient
    });

  } catch (error) {
    console.error('Error updating patient:', error);
    return NextResponse.json({
      success: false,
      message: 'Failed to update patient',
      error: error instanceof Error ? error.message : 'Unknown error'
    }, { status: 500 });
  }
}

// DELETE patient
export async function DELETE(
  request: Request,
  { params }: { params: { id: string } }
) {
  try {
    // Await the params object
    const { id } = await Promise.resolve(params);
    
    await connectDB();

    const patient = await Patient.findByIdAndDelete(id);

    if (!patient) {
      return NextResponse.json({
        success: false,
        message: 'Patient not found'
      }, { status: 404 });
    }

    return NextResponse.json({
      success: true,
      message: 'Patient deleted successfully'
    });

  } catch (error) {
    console.error('Error deleting patient:', error);
    return NextResponse.json({
      success: false,
      message: 'Failed to delete patient',
      error: error instanceof Error ? error.message : 'Unknown error'
    }, { status: 500 });
  }
} 