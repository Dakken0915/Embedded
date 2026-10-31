import React from 'react';
import Layout from '@theme/Layout';
import Link from '@docusaurus/Link';

const sections = [
  { title: 'C、C++ 与构建', text: 'C 语言、C++ 语言、STL、Makefile 与 Shell。', link: '/docs/技术知识/语言与构建/C语言' },
  { title: 'Linux 与嵌入式软件', text: 'Linux 内存、软件分层、Qt 与 ROS2。', link: '/docs/技术知识/Linux与框架/Linux内存管理' },
  { title: '硬件接口与可编程逻辑', text: 'UART、RTC、ADC、SPI、看门狗、Flash、CAN 与 Verilog。', link: '/docs/技术知识/硬件与接口/通用外设/' },
  { title: '算法与数据结构', text: '排序、查找、线性结构、树、图、回溯、贪心与动态规划。', link: '/docs/技术知识/算法与数据结构/数据结构与算法/' },
  { title: '图像处理', text: 'OpenCV 相关技术资料。', link: '/docs/技术知识/图像处理/OpenCV/' },
];

export default function Home() {
  return (
    <Layout title="嵌入式技术知识库" description="嵌入式系统技术资料与知识点">
      <main className="atlas-home">
        <section className="atlas-hero">
          <div className="hero-grid" />
          <div className="hero-copy">
            <p className="eyebrow">EMBEDDED SYSTEMS / TECHNICAL NOTES</p>
            <h1>嵌入式技术<br /><span>知识库</span></h1>
            <p className="hero-lead">嵌入式系统相关技术资料、接口说明、代码示例与算法记录。</p>
            <div className="hero-actions">
              <Link className="button button--primary" to="/docs/技术知识/知识索引">查看知识索引 <span>↗</span></Link>
            </div>
          </div>
        </section>
        <section className="atlas-content">
          <div className="section-intro"><span className="section-kicker">TECHNICAL INDEX</span><h2>知识分类</h2></div>
          <div className="area-grid">
            {sections.map((section, index) => (
              <Link className="area-card area-cyan" to={section.link} key={section.title}>
                <span className="area-number">{String(index + 1).padStart(2, '0')}</span>
                <div><h3>{section.title}</h3><p>{section.text}</p><span className="card-link">查看内容 <b>→</b></span></div>
              </Link>
            ))}
          </div>
        </section>
      </main>
    </Layout>
  );
}
