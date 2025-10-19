import React from 'react';
import { SpeedInsights } from '@vercel/speed-insights/react';
import { Analytics } from '@vercel/analytics/react';

// Default implementation, that you can customize
export default function Root({children}: {children: React.ReactNode}) {
  return (
    <>
      {children}
      <SpeedInsights />
      <Analytics />
    </>
  );
}
