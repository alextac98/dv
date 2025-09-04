import React, {type ReactNode} from 'react';
import clsx from 'clsx';
import Heading from '@theme/Heading';
import styles from './styles.module.css';

type FeatureItem = {
  title: string;
  /**
   * Accept either an SVG React component OR a static asset path (string) such as .png/.webp.
   * When a string is provided it will be rendered inside an <img> tag.
   */
  graphic: React.ComponentType<React.ComponentProps<'svg'>> | string;
  description: ReactNode;
};

const FeatureList: FeatureItem[] = [
  {
    title: 'Easy to Use!',
    graphic: require('@site/static/img/Units.webp').default,
    description: (
      <>
        DV is designed to be easy to use and integrate into your projects, with a
        simple and intuitive API. Never worry about unit bugs again!
      </>
    ),
  },
  {
    title: 'Unit-Safe Math and Conversions',
    graphic: require('@site/static/img/MathConversion.webp').default,
    description: (
      <>
        Converting between units has never been easier, with support for any unit possible!
        When performing math operations, DV automatically verifies units during calculations,
        ensuring consistent and accurate results.
      </>
    ),
  },
  {
    title: 'Multi-Language Support',
    graphic: require('@site/static/img/MultiLanguageSupport.webp').default,
    description: (
      <>
        DimensionalVariables are supported in many programming languages,
        making it easy to integrate DV into your existing codebase.
      </>
    ),
  },
];

function Feature({title, graphic, description}: FeatureItem) {
  const isImage = typeof graphic === 'string';
  const GraphicComponent = (!isImage ? (graphic as React.ComponentType<React.ComponentProps<'svg'>>) : null);
  return (
    <div className={clsx('col col--4')}>
      <div className="text--center">
        {isImage ? (
          <img src={graphic} className={styles.featureSvg} alt={title} />
        ) : (
          GraphicComponent && <GraphicComponent className={styles.featureSvg} role="img" />
        )}
      </div>
      <div className="text--center padding-horiz--md">
        <Heading as="h3">{title}</Heading>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures(): ReactNode {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
