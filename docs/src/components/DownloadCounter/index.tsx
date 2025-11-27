import React, { useEffect, useState } from 'react';
import styles from './styles.module.css';

interface DownloadStats {
  pypi: number;
  crates: number;
  github: number;
  total: number;
}

// Helper function to parse shields.io formatted values (e.g., "1.2k" -> 1200)
function parseShieldsValue(value: string): number {
  const str = value.toLowerCase().trim();
  const multipliers: { [key: string]: number } = {
    'k': 1000,
    'm': 1000000,
    'b': 1000000000
  };
  
  const match = str.match(/^([\d.]+)([kmb]?)$/);
  if (!match) return 0;
  
  const num = parseFloat(match[1]);
  const suffix = match[2];
  
  return Math.round(num * (multipliers[suffix] || 1));
}

export default function DownloadCounter(): React.ReactNode {
  const [stats, setStats] = useState<DownloadStats>({ pypi: 0, crates: 0, github: 0, total: 0 });
  const [displayCount, setDisplayCount] = useState(0);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const fetchStats = async () => {
      try {
        setLoading(true);
        
        let pypiDownloads = 0;
        let cratesDownloads = 0;
        let githubDownloads = 0;

        // Fetch PyPI stats - using shields.io JSON endpoint (has CORS support)
        try {
          const pypiResponse = await fetch('https://img.shields.io/pypi/dm/dv-py.json');
          if (pypiResponse.ok) {
            const pypiData = await pypiResponse.json();
            // Shields.io returns the value as a formatted string like "1.2k"
            // We need to parse it back to a number
            const valueStr = pypiData?.value || '0';
            pypiDownloads = parseShieldsValue(valueStr);
            console.log('PyPI data:', pypiData);
          }
        } catch (err) {
          console.error('PyPI fetch error:', err);
        }

        // Fetch Crates.io stats
        try {
          const cratesResponse = await fetch('https://crates.io/api/v1/crates/dv');
          if (cratesResponse.ok) {
            const cratesData = await cratesResponse.json();
            cratesDownloads = cratesData?.crate?.downloads || 0;
          }
        } catch (err) {
          console.error('Crates.io fetch error:', err);
        }

        // Fetch GitHub releases stats
        try {
          const githubResponse = await fetch('https://api.github.com/repos/alextac98/dv/releases');
          if (githubResponse.ok) {
            const githubData = await githubResponse.json();
            githubDownloads = Array.isArray(githubData) 
              ? githubData.reduce((sum, release) => {
                  const releaseDownloads = release.assets?.reduce((assetSum: number, asset: any) => 
                    assetSum + (asset.download_count || 0), 0) || 0;
                  return sum + releaseDownloads;
                }, 0)
              : 0;
          }
        } catch (err) {
          console.error('GitHub fetch error:', err);
        }

        const total = pypiDownloads + cratesDownloads + githubDownloads;

        // Only show error if all APIs failed
        if (total === 0 && cratesDownloads === 0 && githubDownloads === 0) {
          setError('Unable to fetch download statistics');
        } else {
          setStats({
            pypi: pypiDownloads,
            crates: cratesDownloads,
            github: githubDownloads,
            total: total
          });
        }
        setLoading(false);
      } catch (err) {
        console.error('Error fetching download stats:', err);
        setError('Failed to load download stats');
        setLoading(false);
      }
    };

    fetchStats();
  }, []);

  // Animate counter
  useEffect(() => {
    if (stats.total === 0 || loading) return;

    const duration_ms = 2000; // 2 seconds
    const ms_per_increment = 50;
    const increment = stats.total / (duration_ms / ms_per_increment);
    let current = 0;
    let step = 0;

    const timer = setInterval(() => {
      step++;
      current = Math.min(Math.floor(increment * step), stats.total);
      setDisplayCount(current);

      if (current >= stats.total) {
        clearInterval(timer);
      }
    }, duration_ms / ms_per_increment);

    return () => clearInterval(timer);
  }, [stats.total, loading]);

  if (error) {
    return null;
  }

  return (
    <div className={styles.counterContainer}>
      <div className={styles.counterContent}>
        {loading ? (
          <div className={styles.loading}>Loading stats...</div>
        ) : (
          <>
            <div className={styles.mainCounter}>
              <span className={styles.countNumber}>
                {displayCount.toLocaleString()}
              </span>
              <span className={styles.countLabel}>Downloads And Counting!</span>
            </div>
            {/* <div className={styles.breakdown}>
              <div className={styles.breakdownItem}>
                <span className={styles.breakdownSource}>PyPI</span>
                <span className={styles.breakdownValue}>{stats.pypi.toLocaleString()}</span>
              </div>
              <div className={styles.breakdownItem}>
                <span className={styles.breakdownSource}>Crates.io</span>
                <span className={styles.breakdownValue}>{stats.crates.toLocaleString()}</span>
              </div>
              <div className={styles.breakdownItem}>
                <span className={styles.breakdownSource}>GitHub</span>
                <span className={styles.breakdownValue}>{stats.github.toLocaleString()}</span>
              </div>
            </div> */}
          </>
        )}
      </div>
    </div>
  );
}
