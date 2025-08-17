import { NextResponse } from 'next/server';
import jwt from 'jsonwebtoken';

const JWT_SECRET = process.env.JWT_SECRET || 'your-secret-key';

export async function GET(request: Request) {
  try {
    // Get the cookie from request headers
    const cookieHeader = request.headers.get('cookie');
    const token = cookieHeader
      ?.split(';')
      .find(c => c.trim().startsWith('token='))
      ?.split('=')[1];

    if (!token) {
      return NextResponse.json({ isAuthenticated: false });
    }

    try {
      // Verify the token
      jwt.verify(token, JWT_SECRET);
      return NextResponse.json({ isAuthenticated: true });
    } catch (error) {
      // If token is invalid, clear it
      const response = NextResponse.json({ isAuthenticated: false });
      response.cookies.delete('token');
      return response;
    }
  } catch (error) {
    return NextResponse.json({ isAuthenticated: false });
  }
} 