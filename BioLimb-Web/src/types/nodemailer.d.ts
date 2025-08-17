declare module 'nodemailer' {
  export interface TransportOptions {
    host?: string;
    port?: number;
    secure?: boolean;
    auth?: {
      user: string;
      pass: string;
    };
  }

  export interface MailOptions {
    from: string;
    to: string;
    subject: string;
    html: string;
  }

  export interface SentMessageInfo {
    messageId: string;
  }

  export interface Transporter {
    verify(callback: (error: Error | null, success: boolean) => void): void;
    sendMail(mailOptions: MailOptions): Promise<SentMessageInfo>;
  }

  export function createTransport(options: TransportOptions): Transporter;
} 