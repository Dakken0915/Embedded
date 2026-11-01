import React from 'react';
import { Redirect } from '@docusaurus/router';
import Head from '@docusaurus/Head';
import useBaseUrl from '@docusaurus/useBaseUrl';

export default function IndexRedirect() {
  const target = useBaseUrl('/docs/知识索引');
  return (
    <>
      <Head>
        <meta httpEquiv="refresh" content={`0;url=${target}`} />
      </Head>
      <Redirect to={target} />
    </>
  );
}
