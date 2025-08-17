export const getNegativeTestTemplate = (patientName: string) => ({
  subject: 'Your COVID-19 Test Result - Negative',
  html: `
    <div style="font-family: 'Segoe UI', Arial, sans-serif; max-width: 600px; margin: 0 auto; padding: 40px 20px; background-color: #ffffff;">
      <!-- <div style="text-align: center; margin-bottom: 30px;">
        <img src="https://biolimb.com/logo.png" alt="Biolimb Logo" style="max-width: 150px; height: auto;">
      </div> -->
      
      <div style="background-color: #f8fafc; border-radius: 12px; padding: 30px; box-shadow: 0 4px 6px rgba(0, 0, 0, 0.05);">
        <h2 style="color: #1e40af; font-size: 24px; margin-bottom: 20px; text-align: center;">COVID-19 Test Result</h2>
        
        <div style="background-color: #dcfce7; border-radius: 8px; padding: 20px; margin: 20px 0;">
          <p style="color: #166534; font-size: 18px; margin: 0; text-align: center;">
            <strong>NEGATIVE</strong>
          </p>
        </div>

        <p style="color: #1e293b; font-size: 16px; line-height: 1.6; margin-bottom: 20px;">
          Dear ${patientName},
        </p>
        
        <p style="color: #1e293b; font-size: 16px; line-height: 1.6; margin-bottom: 20px;">
          We are pleased to inform you that your COVID-19 test result is negative. While this is good news, we encourage you to continue following these important safety measures:
        </p>

        <div style="background-color: #ffffff; border-radius: 8px; padding: 20px; margin: 20px 0;">
          <ul style="color: #334155; font-size: 15px; line-height: 1.6; margin: 0; padding-left: 20px;">
            <li style="margin-bottom: 10px;">Wear a mask in public spaces</li>
            <li style="margin-bottom: 10px;">Maintain social distancing</li>
            <li style="margin-bottom: 10px;">Wash hands frequently</li>
            <li style="margin-bottom: 10px;">Monitor for any symptoms</li>
          </ul>
        </div>

        <div style="margin-top: 30px; padding-top: 20px; border-top: 1px solid #e2e8f0;">
          <p style="color: #475569; font-size: 15px; line-height: 1.6;">
            If you have any questions or concerns, please contact us at 
            <a href="mailto:biolimb@robosuperior.com" style="color: #2563eb; text-decoration: none; font-weight: 500;">
              biolimb@robosuperior.com
            </a>
          </p>
        </div>
      </div>

      <div style="text-align: center; margin-top: 30px; color: #64748b; font-size: 14px;">
        <p style="margin: 0;">Best regards,</p>
        <p style="margin: 5px 0 0 0; font-weight: 500;">Biolimb Medical Team</p>
      </div>
    </div>
  `
});

export const getPositiveTestTemplate = (patientName: string) => ({
  subject: 'Your COVID-19 Test Result - Positive',
  html: `
    <div style="font-family: 'Segoe UI', Arial, sans-serif; max-width: 600px; margin: 0 auto; padding: 40px 20px; background-color: #ffffff;">
      <!-- <div style="text-align: center; margin-bottom: 30px;">
        <img src="https://biolimb.com/logo.png" alt="Biolimb Logo" style="max-width: 150px; height: auto;">
      </div> -->
      
      <div style="background-color: #f8fafc; border-radius: 12px; padding: 30px; box-shadow: 0 4px 6px rgba(0, 0, 0, 0.05);">
        <h2 style="color: #1e40af; font-size: 24px; margin-bottom: 20px; text-align: center;">COVID-19 Test Result</h2>
        
        <div style="background-color: #fee2e2; border-radius: 8px; padding: 20px; margin: 20px 0;">
          <p style="color: #991b1b; font-size: 18px; margin: 0; text-align: center;">
            <strong>POSITIVE</strong>
          </p>
        </div>

        <p style="color: #1e293b; font-size: 16px; line-height: 1.6; margin-bottom: 20px;">
          Dear ${patientName},
        </p>
        
        <p style="color: #1e293b; font-size: 16px; line-height: 1.6; margin-bottom: 20px;">
          Your COVID-19 test result is positive. Please follow these important steps immediately:
        </p>

        <div style="background-color: #ffffff; border-radius: 8px; padding: 20px; margin: 20px 0;">
          <ol style="color: #334155; font-size: 15px; line-height: 1.6; margin: 0; padding-left: 20px;">
            <li style="margin-bottom: 10px;">Self-isolate immediately</li>
            <li style="margin-bottom: 10px;">Monitor your symptoms closely</li>
            <li style="margin-bottom: 10px;">Contact emergency services if you experience severe symptoms</li>
            <li style="margin-bottom: 10px;">Inform close contacts about your positive result</li>
          </ol>
        </div>

        <div style="background-color: #fef3c7; border-radius: 8px; padding: 15px; margin: 20px 0;">
          <p style="color: #92400e; font-size: 15px; line-height: 1.6; margin: 0;">
            <strong>Emergency Warning:</strong> If you experience difficulty breathing, persistent chest pain, or confusion, seek emergency medical care immediately.
          </p>
        </div>

        <div style="margin-top: 30px; padding-top: 20px; border-top: 1px solid #e2e8f0;">
          <p style="color: #475569; font-size: 15px; line-height: 1.6;">
            For any questions or assistance, please contact us at 
            <a href="mailto:biolimb@robosuperior.com" style="color: #2563eb; text-decoration: none; font-weight: 500;">
              biolimb@robosuperior.com
            </a>
          </p>
        </div>
      </div>

      <div style="text-align: center; margin-top: 30px; color: #64748b; font-size: 14px;">
        <p style="margin: 0;">Best regards,</p>
        <p style="margin: 5px 0 0 0; font-weight: 500;">Biolimb Medical Team</p>
      </div>
    </div>
  `
}); 