import { NextResponse } from 'next/server';
import jwt from 'jsonwebtoken';

const JWT_SECRET = process.env.JWT_SECRET || 'your-secret-key';

export async function POST(req: Request) {
  try {
    const { token } = await req.json();

    if (!token) {
      return NextResponse.json({ isValid: false, message: 'No token provided' }, { status: 400 });
    }

    try {
      const decoded = jwt.verify(token, JWT_SECRET);
      return NextResponse.json({ isValid: true, user: decoded });
    } catch (error) {
      return NextResponse.json({ isValid: false, message: 'Invalid token' }, { status: 401 });
    }
  } catch (error: any) {
    return NextResponse.json({ isValid: false, message: error.message }, { status: 500 });
  }
} 