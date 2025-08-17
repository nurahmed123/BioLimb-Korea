import nodemailer from 'nodemailer';
import { getNegativeTestTemplate, getPositiveTestTemplate } from './email-templates';

// Create reusable transporter object using SMTP transport
const transporter = nodemailer.createTransport({
  host: process.env.SMTP_HOST,
  port: Number(process.env.SMTP_PORT),
  secure: false, // true for 465, false for other ports
  auth: {
    user: process.env.SMTP_USER,
    pass: process.env.SMTP_PASSWORD,
  },
});

// Verify SMTP connection configuration
transporter.verify(function (error: Error | null, success: boolean) {
  if (error) {
    console.error('SMTP Configuration Error:', error);
  } else {
    console.log('SMTP Server is ready to take our messages');
  }
});

export async function sendTestResultEmail(
  to: string,
  patientName: string,
  testResult: 'Positive' | 'Negative'
): Promise<boolean> {
  try {
    console.log('Preparing to send email with configuration:', {
      host: process.env.SMTP_HOST,
      port: process.env.SMTP_PORT,
      user: process.env.SMTP_USER,
      // Don't log the password
    });

    // Select template based on test result
    const template = testResult === 'Positive' 
      ? getPositiveTestTemplate(patientName)
      : getNegativeTestTemplate(patientName);
    
    // Configure email options
    const mailOptions = {
      from: `"BioLimb" <${process.env.SMTP_USER}>`,
      to: to,
      subject: template.subject,
      html: template.html
    };

    console.log('Sending email to:', to);
    
    // Send email
    const info = await transporter.sendMail(mailOptions);
    console.log('Email sent successfully:', info.messageId);
    return true;
  } catch (error) {
    console.error('Error sending email:', error);
    return false;
  }
} 