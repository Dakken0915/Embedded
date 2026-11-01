// @ts-check
import { themes as prismThemes } from 'prism-react-renderer';

/** @type {import('@docusaurus/types').Config} */
const config = {
  title: '嵌入式技术知识库',
  tagline: '嵌入式系统技术资料与知识点',
  favicon: 'img/favicon.svg',
  url: process.env.SITE_IP ? `http://${process.env.SITE_IP}:3000` : 'http://127.0.0.1:3000',
  baseUrl: '/',
  markdown: { format: 'md' },
  onBrokenLinks: 'warn',
  onBrokenMarkdownLinks: 'warn',
  i18n: { defaultLocale: 'zh-Hans', locales: ['zh-Hans'] },
  presets: [
    ['classic', {
      docs: {
        path: './docs',
        sidebarPath: './sidebars.js',
        routeBasePath: 'docs',
      },
      blog: false,
      theme: { customCss: './src/css/custom.css' },
    }],
  ],
  themeConfig: {
    colorMode: { defaultMode: 'dark', disableSwitch: false, respectPrefersColorScheme: false },
    navbar: {
      title: '嵌入式技术知识库',
      logo: { alt: '嵌入式技术知识库', src: 'img/favicon.svg' },
      items: [
        { to: '/docs/知识索引', label: '知识索引', position: 'left' },
      ],
    },
    prism: { theme: prismThemes.github, darkTheme: prismThemes.dracula },
    footer: {
      style: 'dark',
      links: [
        { title: '导航', items: [
          { label: '知识索引', to: '/docs/知识索引' },
        ] },
        { title: '资料', items: [
          { label: 'C++ STL', to: '/docs/语言与构建/C++ STL' },
        ] },
      ],
      copyright: `嵌入式技术知识库 · ${new Date().getFullYear()}`,
    },
  },
};

export default config;
