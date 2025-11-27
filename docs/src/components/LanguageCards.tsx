import React, { useState } from 'react';
import styles from './LanguageCards.module.css';

interface LanguageCardProps {
  name: string;
  icon: string;
  packageManager: string;
  packageCommand: string;
  docLink: string;
  isComingSoon: boolean;
}

const languages: LanguageCardProps[] = [
  {
    name: 'Rust',
    icon: '🦀',
    packageManager: 'Cargo',
    packageCommand: 'cargo add dv',
    docLink: '/docs/rust',
    isComingSoon: false,
  },
  {
    name: 'Python',
    icon: '🐍',
    packageManager: 'pip',
    packageCommand: 'pip install dv_py',
    docLink: '/docs/python',
    isComingSoon: false,
  },
  {
    name: 'Bazel',
    icon: '🛠️',
    packageManager: 'Bazel',
    packageCommand: `bazel_dep(name = "dv", version = "0.0.0")
git_override(
    module_name = "dv",
    remote = "https://github.com/alextac98/dv.git",
    commit = "main",
)`,
    docLink: '/docs/c_cpp#bazel-with-c',
    isComingSoon: false,
  },
  {
    name: 'C/C++ (CMake)',
    icon: '⚙️',
    packageManager: 'CMake',
    packageCommand: `include(FetchContent)
FetchContent_Declare(dv GIT_REPOSITORY https://github.com/alextac98/dv.git GIT_TAG main)
FetchContent_MakeAvailable(dv)`,
    docLink: '/docs/c_cpp#cmake-with-c',
    isComingSoon: false,
  },
];

function LanguageCard({ name, icon, packageManager, packageCommand, docLink, isComingSoon}: LanguageCardProps) {
  const [copied, setCopied] = useState(false);

  const handleCopy = async (e: React.MouseEvent) => {
    e.preventDefault();
    e.stopPropagation();
    
    try {
      await navigator.clipboard.writeText(packageCommand);
      setCopied(true);
      setTimeout(() => setCopied(false), 2000);
    } catch (err) {
      console.error('Failed to copy text:', err);
    }
  };

  const CardContent = (
    <div className={styles.card}>
      <div className={styles.cardHeader}>
        <span className={styles.icon}>{icon}</span>
        <h3 className={styles.langName}>{name}</h3>
      </div>
      <div className={styles.cardBody}>
        {isComingSoon ? (
          <div className={styles.comingSoonBadge}>🚧 Coming Soon 🚧</div>
        ) : (
          <>
            <div 
              className={styles.codeBlock}
              onClick={(e) => {
                e.preventDefault();
                e.stopPropagation();
              }}
              onMouseDown={(e) => {
                e.stopPropagation();
              }}
            >
              <button
                className={styles.copyButton}
                onClick={handleCopy}
                onMouseDown={(e) => {
                  e.stopPropagation();
                }}
                title="Copy to clipboard"
              >
                {copied ? (
                  <svg width="16" height="16" viewBox="0 0 16 16" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M13.5 3.5L6 11L2.5 7.5" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"/>
                  </svg>
                ) : (
                  <svg width="16" height="16" viewBox="0 0 16 16" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M10.5 2.5H12.5C13.0523 2.5 13.5 2.94772 13.5 3.5V13.5C13.5 14.0523 13.0523 14.5 12.5 14.5H5.5C4.94772 14.5 4.5 14.0523 4.5 13.5V12.5" stroke="currentColor" strokeWidth="1.5" strokeLinecap="round" strokeLinejoin="round"/>
                    <rect x="2.5" y="2.5" width="7" height="9" rx="1" stroke="currentColor" strokeWidth="1.5"/>
                  </svg>
                )}
              </button>
              <textarea 
                readOnly 
                value={packageCommand}
                rows={packageCommand.split('\n').length}
                onClick={(e) => {
                  e.preventDefault();
                  e.stopPropagation();
                  e.currentTarget.select();
                }}
                onMouseDown={(e) => {
                  e.stopPropagation();
                }}
              />
            </div>
          </>
        )}
      </div>
      <div className={styles.cardFooter}>
        {isComingSoon ? (
          <span className={styles.docLink}>
            Stay Tuned!
          </span>
        ) : (
          <span className={styles.docLink}>
            View Documentation →
          </span>
        )}
      </div>
    </div>
  );

  return (
    <a href={docLink} className={styles.cardLink}>
      {CardContent}
    </a>
  );
}

export default function LanguageCards() {
  return (
    <section className={styles.languageCards}>
      <div className="container">
        <h2 className={styles.title}>Supported Languages</h2>
        <p className={styles.subtitle}>
          Choose your preferred language and get started with dimensional variables!
        </p>
        <div className={styles.cardsContainer}>
          {languages.map((lang) => (
            <LanguageCard key={lang.name} {...lang} />
          ))}
        </div>
      </div>
    </section>
  );
}